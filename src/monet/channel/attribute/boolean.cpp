//
// Created by maxng on 12/13/2023.
//

#include <monet.hpp>

namespace monet::channel::attribute {

    std::span<std::string_view const> boolean::available_channels() const noexcept {
        static std::vector<std::string_view> const m_available_channels = {
            "base",
            "binary",
            "inverse"
        };

        return m_available_channels;
    }

    uint8_t boolean::value(attribute_channel const a_channel) const noexcept {
        switch (a_channel) {
            case binary:
                return m_state;
            case inverse:
                return !m_state * 255;
            case base:
            default:
                return m_state * 255;
        }
    }

    void boolean::set_value(attribute_channel const a_channel, uint8_t const a_value) noexcept {
        switch (a_channel) {
            case binary:
                // Translate to 0-255.
                m_state = a_value > 0;
                break;
            case inverse:
                m_state = a_value < boolean_threshold;
            case base:
            default:
                m_state = a_value >= boolean_threshold;
        }
    }

    registry::register_attribute<"boolean", boolean> _boolean_registry;

}