//
// Created by maxng on 3/12/2024.
//

#ifndef SQLITE_ADAPTER_HPP
#define SQLITE_ADAPTER_HPP

#include <filesystem>
#include <utility>

#include <SQLiteCpp/SQLiteCpp.h>
#include <fmt/format.h>

#include "../adapter.hpp"

namespace monet::storage {

    class sqlite_adapter;

    class sqlite_document final : public document {
        SQLite::Database& m_database;
        std::string m_name;
        SQLite::Statement m_set_query;
        mutable SQLite::Statement m_get_query;
        mutable std::string m_key;
        mutable std::string m_value;
        mutable SQLite::Statement m_all_query;
        mutable SQLite::Statement m_range_query;

        friend class sqlite_adapter;

        sqlite_document(SQLite::Database& a_database, std::string a_name);

    public:
        void set(std::string_view a_key, nlohmann::json a_data) noexcept override;

        [[nodiscard]]
        std::optional<nlohmann::json> get(std::string_view a_key) const noexcept override;

        [[nodiscard]]
        bool exists(std::string_view a_key) const noexcept override;

        [[nodiscard]]
        std::map<std::string, nlohmann::json> all() const noexcept override;

        [[nodiscard]]
        std::vector<std::pair<std::string, nlohmann::json>> range(size_t a_start, size_t a_count) const noexcept override;
    };

    class sqlite_adapter final : public adapter {
        std::filesystem::path m_database_file;
        std::unique_ptr<SQLite::Database> m_database;

    public:
        explicit sqlite_adapter(std::filesystem::path a_database_file) noexcept;

        /**
        * @brief Create a new document.
        *
        * @param a_name The name of the document to create.
        *
        * @return The new document with the given name.
        *
        * @note If a document with the specified name already exists, the existing document will be returned.
        */
        std::shared_ptr<document> create_doc(std::string a_name) noexcept override;

        /**
         * @brief Check if a document exists.
         *
         * @param a_name The name for which to check.
         *
         * @return True if the file exists, false if not.
         */
        [[nodiscard]]
        bool doc_exists(std::string_view a_name) const noexcept override;

        /**
         * @brief Fetch a document if it exists.
         *
         * @param a_name The name of the document to check.
         *
         * @return The document with the given name or a null optional if not.
         */
        [[nodiscard]]
        std::optional<std::shared_ptr<document>> fetch_doc(std::string_view a_name) noexcept override;

    private:
        void create_table(std::string_view a_name) noexcept;
    };

}

#endif //SQLITE_ADAPTER_HPP
