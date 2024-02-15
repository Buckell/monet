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
        if (auto const it = m_attributes.find(a_type); it == m_attributes.cend()) {
            return std::span<std::unique_ptr<attribute::attribute>>{};
        } else {
            return it->second;
        }
    }

    std::span<std::unique_ptr<attribute::attribute> const> channel::attributes(std::string_view const a_type) const noexcept {
        if (auto const it = m_attributes.find(a_type); it == m_attributes.cend()) {
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
            if (auto const it = m_attributes.find(type); it != m_attributes.cend()) {
                if (channel.empty()) {
                    address_values.emplace_back(it->second[index]->value(0));
                } else {
                    auto const id = it->second[index]->channel_name_to_id(channel);
                    address_values.emplace_back(it->second[index]->value(id));
                }
            } else {
                address_values.emplace_back(0);
            }
        }

        return address_values;
    }

    void channel::find_universe() {
        if (!m_server || m_base_address == 0) {
            m_universe = nullptr;
            m_address = 0;
            return;
        }

        auto [universe, address] = address::from_master_id(m_base_address);

        m_universe = &m_server->get_universe(universe);
        m_address = address;
    }

    void channel::push_updates() const {
        if (!m_universe) {
            return;
        }

        auto [address, values] = address_info();

        m_universe->set_addresses(address, values);
    }

    void channel::set_intensity(uint8_t const a_intensity) noexcept {
        if (auto const intensities = attributes("intensity"); !intensities.empty()) {
            intensities[0]->set_value(monet::channel::attribute::intensity::normal, a_intensity);

            push_updates();
        }
    }

    void channel::set_rgb_color(uint8_t const a_red, uint8_t const a_green, uint8_t const a_blue) noexcept {
        if (auto const rgb_colors = attributes("rgb_color"); !rgb_colors.empty()) {
            auto const& rgb_color = rgb_colors[0];
            rgb_color->set_value(monet::channel::attribute::rgb_color::red,   a_red);
            rgb_color->set_value(monet::channel::attribute::rgb_color::green, a_green);
            rgb_color->set_value(monet::channel::attribute::rgb_color::blue,  a_blue);
        }

        push_updates();
    }

    void channel::set_rgb_color(uint32_t const a_color) noexcept {
        union {
            uint32_t value;
            uint8_t channel[4];
        } color{};

        color.value = a_color;

        set_rgb_color(color.channel[2], color.channel[1], color.channel[0]);
    }

}