//
// Created by maxng on 3/11/2024.
//

#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

namespace monet::storage {

    class document {
    public:
        virtual ~document() = default;

        virtual void set(std::string_view a_key, nlohmann::json a_data) noexcept = 0;

        [[nodiscard]]
        virtual std::optional<nlohmann::json> get(std::string_view a_key) const noexcept = 0;

        [[nodiscard]]
        virtual bool exists(std::string_view a_key) const noexcept = 0;

        [[nodiscard]]
        virtual std::map<std::string, nlohmann::json> all() const noexcept = 0;

        [[nodiscard]]
        virtual std::vector<std::pair<std::string, nlohmann::json>> range(size_t a_start, size_t a_count) const noexcept = 0;

        [[nodiscard]]
        std::vector<std::pair<std::string, nlohmann::json>> page(size_t const a_page, size_t const a_entry_count = 10) const noexcept {
            return range(a_page * a_entry_count, a_entry_count);
        }
    };

}

#endif //DOCUMENT_HPP
