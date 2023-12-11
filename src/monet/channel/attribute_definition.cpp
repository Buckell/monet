//
// Created by maxng on 12/10/2023.
//

#include <monet.hpp>

namespace monet::channel {

    std::string_view attribute_definition::property(std::string_view const a_property_name) const noexcept {
        auto it = m_properties.find(a_property_name);

        if (it == m_properties.cend()) {
            return {};
        } else {
            return it->second;
        }
    }

    void attribute_definition::set_property(std::string a_property_name, std::string a_property_value) noexcept {
        m_properties.emplace(std::move(a_property_name), std::move(a_property_value));
    }

}