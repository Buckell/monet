//
// Created by maxng on 3/12/2024.
//

#ifndef JSON_ADAPTER_HPP
#define JSON_ADAPTER_HPP

#include <filesystem>
#include <utility>
#include <fstream>

#include "../adapter.hpp"

namespace monet::storage {

    class json_adapter;

    class json_document final : public document {
        json_adapter& m_host;
        std::string m_name;
        std::filesystem::path m_path;
        nlohmann::json m_data;

        friend class json_adapter;

        json_document(json_adapter& a_host, std::string a_name) noexcept;

        ~json_document() override;

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

    private:
        void write() const noexcept;
        void read() noexcept;
    };

    class json_adapter final : public adapter {
        std::filesystem::path m_base_directory;
        std::map<std::string, std::reference_wrapper<std::shared_ptr<document>>> m_open_documents;

        friend class json_document;

    public:
        explicit json_adapter(std::filesystem::path a_base_directory) noexcept :
            m_base_directory(std::move(a_base_directory))
        {
            create_directories(m_base_directory);
        }

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
        void return_document(std::string const& a_name) {
            m_open_documents.erase(a_name);
        }

        void checkout(std::string a_name, std::shared_ptr<document>& a_document) {
            m_open_documents.emplace(std::move(a_name), a_document);
        }
    };

}

#endif //JSON_ADAPTER_HPP
