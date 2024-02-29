//
// Created by maxng on 12/6/2023.
//

#ifndef MASTER_SERVER_BOOLEAN_HPP
#define MASTER_SERVER_BOOLEAN_HPP

#include <vector>
#include <string_view>
#include <span>
#include <cmath>

#include "attribute.hpp"

namespace monet::channel::attribute {

    /**
     * @brief An attribute implementation representing a boolean true/false, on/off, etc. state.
     *
     * <table>
     *      <caption>Attribute Channels</caption>
     *      <tr> <th>Channel Name  <th> ID    <th>Return   <th>Description
     *
     *      <tr> <td>base          <td> 0x0   <td>0/255    <td>A value of 0 or 255 depending on state; 0 = false/off, 255 = true/on.
     *      <tr> <td>binary        <td> 0x1   <td>0/1      <td>A value of 0 or 1 depending on state; 0 = false/off, 1 = true/on.
     *      <tr> <td>inverse       <td> 0x1   <td>255/0    <td>A value of 255 or 0 depending on the inverse of the state; 255 = false/off, 0 = true/on.
     */
    class boolean : public attribute {
        bool m_state;

    public:
        constexpr static attribute_channel binary = 0x1;
        constexpr static attribute_channel inverse = 0x2;

        constexpr static size_t boolean_threshold = 127;

        boolean() :
            attribute("boolean"),
            m_state(false)
        {}

        boolean(boolean const&) noexcept = delete;
        boolean(boolean&&) noexcept      = delete;

        boolean& operator = (boolean const&) = delete;
        boolean& operator = (boolean&&)      = delete;

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
         * @brief Get the internal state value of true or false.
         *
         * @return The internal state value.
         */
        [[nodiscard]]
        bool state_value() const noexcept {
            return m_state;
        }

        /**
         * @brief Set the state value of true or false.
         *
         * @param a_value The value to set the internal state value.
         */
        void set_intensity_value(bool const a_value) noexcept {
            m_state = a_value;
        }
    };

}

#endif //MASTER_SERVER_BOOLEAN_HPP
