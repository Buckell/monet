//
// Created by maxng on 12/21/2023.
//

#include <monet.hpp>

namespace monet::channel {

    size_t channel::attribute_count() const noexcept {
        return m_attributes.size();
    }

    size_t channel::attribute_count(std::string_view const a_attribute_type) const noexcept {
        return std::accumulate(
            m_attributes.begin(),
            m_attributes.end(),
            0u,
            [] (size_t const a_total, auto const& a_next) -> size_t {
                return a_total + a_next.second.size();
            }
        );
    }

    std::span<std::unique_ptr<attribute::attribute>> channel::attributes(std::string_view const a_type) noexcept {
        auto it = m_attributes.find(a_type);

        if (it == m_attributes.cend()) {
            return std::span<std::unique_ptr<attribute::attribute>>{};
        } else {
            return it->second;
        }
    }

    std::span<std::unique_ptr<attribute::attribute> const> channel::attributes(std::string_view const a_type) const noexcept {
        auto it = m_attributes.find(a_type);

        if (it == m_attributes.cend()) {
            return std::span<std::unique_ptr<attribute::attribute>>{};
        } else {
            return it->second;
        }
    }

    void channel::propagate_attributes() {
        // Clear any preexisting attributes.
        m_attributes.clear();

        auto const& attribute_definitions = m_configuration.attributes();

        // Iterate through each defined attribute type.
        for (auto const& [attribute_type, attributes] : attribute_definitions) {
            std::vector<std::unique_ptr<attribute::attribute>> generated_attributes;

            // Iterate through each attribute definition in each type.
            for (auto const& attribute_entry : attributes) {
                generated_attributes.emplace_back(attribute::registry::instantiate_attribute(attribute_type));
            }

            // Transfer generated attributes into the map.
            m_attributes.emplace(
                attribute_type,
                std::move(generated_attributes)
            );
        }
    }

    std::vector<uint8_t> channel::fetch_address_values() const noexcept {
        std::vector<uint8_t> address_values;
        address_values.reserve(attribute_count());

        for (auto const& [type, index, channel] : m_configuration.address_mappings()) {
            auto it = m_attributes.find(type);

            if (it != m_attributes.cend()) {
                if (channel.empty()) {
                    address_values.emplace_back(it->second[index]->value(0));
                } else {
                    auto id = it->second[index]->channel_name_to_id(channel);
                    address_values.emplace_back(it->second[index]->value(id));
                }
            } else {
                address_values.emplace_back(0);
            }
        }

        return address_values;
    }

}