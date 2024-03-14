//
// Created by maxng on 3/12/2024.
//

#include <monet.hpp>

namespace monet::storage {

    json_document::json_document(json_adapter& a_host, std::string a_name) noexcept :
        m_host(a_host),
        m_name(std::move(a_name)),
        m_path(m_host.m_base_directory / (m_name + ".json"))
    {
        read();

        if (m_data.empty()) {
            write();
        }
    }

    json_document::~json_document() {
        m_host.return_document(m_name);
    }

    void json_document::set(std::string_view a_key, nlohmann::json a_data) noexcept {
        m_data[a_key] = std::move(a_data);
        write();
    }

    std::optional<nlohmann::json> json_document::get(std::string_view a_key) const noexcept {
        if (auto const it = m_data.find(a_key); it != m_data.cend()) {
            return it.value();
        }

        return std::nullopt;
    }

    bool json_document::exists(std::string_view a_key) const noexcept {
        return m_data.find(a_key) != m_data.cend();
    }

    std::map<std::string, nlohmann::json> json_document::all() const noexcept {
        std::map<std::string, nlohmann::json> entries;

        for (auto const& [key, value] : m_data.items()) {
            entries.emplace(key, value);
        }

        return entries;
    }

    std::vector<std::pair<std::string, nlohmann::json>> json_document::range(size_t const a_start, size_t const a_count) const noexcept {
        std::vector<std::pair<std::string, nlohmann::json>> entries;

        for (
            auto it = m_data.begin() + static_cast<nlohmann::detail::iter_impl<const nlohmann::basic_json<>>::difference_type>(a_start);
            it != m_data.begin() + static_cast<nlohmann::detail::iter_impl<const nlohmann::basic_json<>>::difference_type>(a_start + a_count);
            ++it
        ) {
            entries.emplace_back(it.key(), it.value());
        }

        return entries;
    }

    void json_document::write() const noexcept {
        std::ofstream file(m_path);
        file << to_string(m_data);
    }

    void json_document::read() noexcept {
        if (std::filesystem::exists(m_path)) {
            std::ifstream file(m_path);
            m_data = nlohmann::json::parse(file);
        }
    }

    std::shared_ptr<document> json_adapter::create_doc(std::string a_name) noexcept {
        std::shared_ptr<document> doc(dynamic_cast<document*>(new json_document(*this, a_name)));
        checkout(std::move(a_name), doc);
        return doc;
    }

    bool json_adapter::doc_exists(std::string_view const a_name) const noexcept {
        return exists(m_base_directory / (std::string(a_name) + ".json"));
    }

    std::optional<std::shared_ptr<document>> json_adapter::fetch_doc(std::string_view const a_name) noexcept {
        std::string name(a_name);

        if (auto const it = m_open_documents.find(name); it != m_open_documents.cend()) {
            return it->second;
        }

        if (doc_exists(name)) {
            std::shared_ptr<document> doc(dynamic_cast<document*>(new json_document(*this, name)));
            checkout(std::move(name), doc);
            return doc;
        }

        return std::nullopt;
    }

}