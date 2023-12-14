//
// Created by maxng on 12/13/2023.
//

#include <monet.hpp>

namespace monet::channel::attribute {

    std::span<std::string_view const> rgb_color::available_channels() const noexcept {
        static std::vector<std::string_view> const m_available_channels = {
            "base", // Base channel not used in rgb_color but must be defined regardless.
            "red",
            "green",
            "blue"
        };

        return m_available_channels;
    }

    uint8_t rgb_color::value(attribute_channel const a_channel) const noexcept {
        switch (a_channel) {
            case red:
                return m_red;
            case green:
                return m_green;
            case blue:
                return m_blue;
            default:
                return 0;
        }
    }

    void rgb_color::set_value(attribute_channel const a_channel, uint8_t const a_value) noexcept {
        switch (a_channel) {
            case red:
                m_red = a_value;
            case green:
                m_green = a_value;
            case blue:
                m_blue = a_value;
                break;
            default:
                break;
        }
    }

}