//
// Created by maxng on 12/10/2023.
//

#include <monet.hpp>

namespace monet::channel {

    size_t configuration::attribute_count() const noexcept {
        return std::accumulate(
            m_attribute_definitions.begin(),
            m_attribute_definitions.end(),
            0u,
            [] (size_t const a_total, auto const& a_next) -> size_t {
                return a_total + a_next.second.size();
            }
        );
    }

    size_t configuration::attribute_count(std::string_view const a_attribute_type) const noexcept {
        auto it = m_attribute_definitions.find(a_attribute_type);

        if (it == m_attribute_definitions.cend()) {
            return 0;
        } else {
            return it->second.size();
        }
    }

    void configuration::add_attribute(std::string a_type, attribute_definition a_attribute_definition) {
        m_attribute_definitions[std::move(a_type)].emplace_back(std::move(a_attribute_definition));
    }

    std::span<attribute_definition> configuration::attributes(std::string_view const a_type) noexcept {
        auto it = m_attribute_definitions.find(a_type);

        if (it == m_attribute_definitions.cend()) {
            return std::span<attribute_definition>{};
        } else {
            return it->second;
        }
    }

    std::span<attribute_definition const> configuration::attributes(std::string_view const a_type) const noexcept {
        auto it = m_attribute_definitions.find(a_type);

        if (it == m_attribute_definitions.cend()) {
            return std::span<attribute_definition>{};
        } else {
            return it->second;
        }
    }

    address_mapping& configuration::mapping(size_t const a_address_index) {
        if (a_address_index >= address_count()) [[unlikely]] {
            throw std::out_of_range("address index exceeds amount of addresses");
        }

        return m_address_mappings[a_address_index];
    }

    address_mapping const& configuration::mapping(size_t a_address_index) const {
        if (a_address_index >= address_count()) [[unlikely]] {
            throw std::out_of_range("address index exceeds amount of addresses");
        }

        return m_address_mappings.at(a_address_index);
    }

}