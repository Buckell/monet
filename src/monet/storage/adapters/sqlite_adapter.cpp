//
// Created by maxng on 3/12/2024.
//

#include <monet.hpp>

namespace monet::storage {

    sqlite_document::sqlite_document(SQLite::Database& a_database, std::string a_name) :
        m_database(a_database),
        m_name(std::move(a_name)),
        m_set_query(m_database, fmt::format("REPLACE INTO {} VALUES (?, ?)", m_name)),
        m_get_query(m_database, fmt::format("SELECT doc_value FROM {} WHERE doc_key=?", m_name)),
        m_all_query(m_database, fmt::format("SELECT * FROM {}", m_name)),
        m_range_query(m_database, fmt::format("SELECT * FROM {} LIMIT ?, ?", m_name))
    {}

    void sqlite_document::set(std::string_view const a_key, nlohmann::json const a_data) noexcept {
        m_key = a_key;
        m_set_query.bind(1, m_key);

        m_value = to_string(a_data);
        m_set_query.bind(2, m_value);

        m_set_query.exec();
        m_set_query.reset();
    }

    std::optional<nlohmann::json> sqlite_document::get(std::string_view const a_key) const noexcept {
        m_key = a_key;
        m_get_query.bind(1, m_key);

        if (m_get_query.executeStep()) {
            std::string data{ m_get_query.getColumn(0).getString() };
            m_get_query.reset();
            return nlohmann::json::parse(data);
        }

        return std::nullopt;
    }

    bool sqlite_document::exists(std::string_view const a_key) const noexcept {
        m_key = a_key;

        m_get_query.exec();

        bool const res = m_get_query.executeStep();
        m_get_query.reset();

        return res;
    }

    std::map<std::string, nlohmann::json> sqlite_document::all() const noexcept {
        std::map<std::string, nlohmann::json> entries;

        while (m_all_query.executeStep()) {
            entries.emplace(m_all_query.getColumn(0).getString(), nlohmann::json::parse(m_all_query.getColumn(1).getString()));
        }

        return entries;
    }

    std::vector<std::pair<std::string, nlohmann::json>> sqlite_document::range(size_t const a_start, size_t const a_count) const noexcept {
        std::vector<std::pair<std::string, nlohmann::json>> entries;

        auto const start = static_cast<int64_t>(a_start);
        auto const count = static_cast<int64_t>(a_count);

        m_range_query.bind(1, start);
        m_range_query.bind(2, count);

        while (m_range_query.executeStep()) {
            entries.emplace_back(m_range_query.getColumn(0).getString(), nlohmann::json::parse(m_range_query.getColumn(1).getString()));
        }

        return entries;
    }

    sqlite_adapter::sqlite_adapter(std::filesystem::path a_database_file) noexcept :
        m_database_file(std::move(a_database_file))
    {
        create_directories(std::filesystem::path(m_database_file).remove_filename());
        m_database = std::make_unique<SQLite::Database>(m_database_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    }

    std::shared_ptr<document> sqlite_adapter::create_doc(std::string a_name) noexcept {
        if (!doc_exists(a_name)) {
            create_table(a_name);
        }

        return std::shared_ptr<document>(new sqlite_document(*m_database, std::move(a_name)));
    }

    std::optional<std::shared_ptr<document>> sqlite_adapter::fetch_doc(std::string_view a_name) noexcept {
        return std::nullopt;
    }

    bool sqlite_adapter::doc_exists(std::string_view const a_name) const noexcept {
        return m_database->tableExists(std::string(a_name));
    }

    void sqlite_adapter::create_table(std::string_view const a_name) noexcept {
        m_database->exec(fmt::format(R"(
            create table {0}
            (
                doc_key   TINYTEXT          not null
                    constraint {0}_index_key
                        primary key,
                doc_value TEXT default '{{}}' not null
            );
        )", a_name));
    }


}