//
// Created by maxng on 12/6/2023.
//

#ifndef MASTER_SERVER_INTENSITY_HPP
#define MASTER_SERVER_INTENSITY_HPP

#include <vector>
#include <string_view>
#include <span>
#include <cmath>

#include "attribute.hpp"

namespace monet::channel::attribute {

    /**
     * @brief An attribute implementation representing the intensity of a light or other dimming fixture.
     *
     * <table>
     *      <caption>Attribute Channels</caption>
     *      <tr> <th>Channel Name  <th> ID    <th>Return   <th>Description
     *
     *      <tr> <td>default       <td> 0x0   <td>0-255    <td>A value of 0-255 based on intensity; 0 = out (0%), 127 = half (50%), 255 = full (100%).
     *      <tr> <td>exact         <td> 0x1   <td>0-100    <td>A more human-readable value of 0-100 based on percentage of intensity; 0 = out (0%), 50 = half (50%), 100 = full (100%).
     */
    class intensity : public attribute {
        /// A 0-255 value representing intensity (0-255 -> 0%-100%).
        uint8_t m_intensity_value;

    public:
        constexpr static attribute_channel normal = 0x1;

        intensity() :
            attribute("intensity"),
            m_intensity_value(0)
        {}

        intensity(intensity const&) noexcept = delete;
        intensity(intensity&&) noexcept      = delete;

        intensity& operator = (intensity const&) = delete;
        intensity& operator = (intensity&&)      = delete;

        /**
         * @brief Get a list of available attribute channels.
         *
         * @note Overrides pure-virtual attribute::available_channels().
         */
        [[nodiscard]]
        std::span<std::string_view const> available_channels() const noexcept override;

        /**
         * @brief Get the value of an attribute channel.
         *
         * @param a_channel The channel name.
         *
         * @return The value of the channel.
         *
         * @note Overrides pure-virtual attribute::value().
         */
        [[nodiscard]]
        uint8_t value(attribute_channel a_channel) const noexcept override;

        /**
         * @brief Set the value of a channel.
         *
         * @param a_channel The channel name.
         * @param a_value The value to set the channel.
         *
         * @note Overrides pure-virtual attribute::set_value().
         */
        void set_value(attribute_channel a_channel, uint8_t a_value) noexcept override;

        /**
         * @brief Get the internal intensity value of 0-255 -> 0%-100%.
         *
         * @return The internal intensity value.
         *
         * Get the internal intensity value, which is a range from 0-255, representing 0%-100%, respectively. This is
         * more efficient than calling the virtual version, value("default").
         */
        [[nodiscard]]
        uint8_t intensity_value() const noexcept {
            return m_intensity_value;
        }

        /**
         * @brief Set the internal intensity value of 0-255 -> 0%-100%.
         *
         * @param a_value The value to set the internal intensity value.
         *
         * Set the internal intensity value, which is a range from 0-255, representing 0%-100%, respectively. This is
         * more efficient than calling the virtual version, set_value("default", ...).
         */
        void set_intensity_value(uint8_t const a_value) noexcept {
            m_intensity_value = a_value;
        }
    };

}

#endif //MASTER_SERVER_INTENSITY_HPP
