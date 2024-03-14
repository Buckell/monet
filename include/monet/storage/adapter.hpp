//
// Created by maxng on 3/11/2024.
//

#ifndef ADAPTER_HPP
#define ADAPTER_HPP

#include <memory>

#include "document.hpp"

namespace monet::storage {

    class adapter {

    public:
        adapter() = default;
        virtual ~adapter() = default;

        /**
        * @brief Create a new document.
        *
        * @param a_name The name of the document to create.
        *
        * @return The new document with the given name.
        *
        * @note If a document with the specified name already exists, the existing document will be returned.
        */
        virtual std::shared_ptr<document> create_doc(std::string a_name) noexcept = 0;

        /**
         * @brief Check if a document exists.
         *
         * @param a_name The name for which to check.
         *
         * @return True if the file exists, false if not.
         */
        [[nodiscard]]
        virtual bool doc_exists(std::string_view a_name) const noexcept = 0;

        /**
         * @brief Fetch a document if it exists.
         *
         * @param a_name The name of the document to check.
         *
         * @return The document with the given name or a null optional if not.
         */
        [[nodiscard]]
        virtual std::optional<std::shared_ptr<document>> fetch_doc(std::string_view a_name) noexcept = 0;

        /**
        * @brief Create/fetch a document. Analagous to create_doc().
        *
        * @param a_name The name of the document to retreive/create.
        *
        * @return The document with the given name.
        */
        std::shared_ptr<document> doc(std::string_view a_name) noexcept {
            return create_doc(std::string(a_name));
        }
    };

}

#endif //ADAPTER_HPP
