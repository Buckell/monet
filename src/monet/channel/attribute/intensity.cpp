//
// Created by maxng on 12/13/2023.
//

#include <monet.hpp>

namespace monet::channel::attribute {

    std::span<std::string_view const> intensity::available_channels() const noexcept {
        static std::vector<std::string_view> const m_available_channels = {
            "base",  // 0-255
            "normal" // 0-100
        };

        return m_available_channels;
    }

    uint8_t intensity::value(attribute_channel const a_channel) const noexcept {
        switch (a_channel) {
            case normal:
                // Translate to 0-100.
                return static_cast<uint8_t>(std::ceil((static_cast<double>(m_intensity_value) / 255.0) * 100.0));
            case base:
            default:
                return m_intensity_value;
        }
    }

    void intensity::set_value(attribute_channel const a_channel, uint8_t const a_value) noexcept {
        switch (a_channel) {
            case normal:
                // Translate to 0-255.
                m_intensity_value = static_cast<uint8_t>((static_cast<double>(a_value) / 100.0) * 255.0);
                break;
            case base:
            default:
                m_intensity_value = a_value;
        }
    }

    registry::register_attribute<"intensity", intensity> _intensity_registry;

}