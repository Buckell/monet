//
// Created by maxng on 12/13/2023.
//

#ifndef MASTER_SERVER_COLOR_HPP
#define MASTER_SERVER_COLOR_HPP

#include "attribute.hpp"

namespace monet::channel::attribute {

    /**
     * @brief An attribute implementation representing an RGB color.
     *
     * <table>
     *      <caption>Attribute Channels</caption>
     *      <tr> <th>Channel Name  <th> ID    <th>Return   <th>Description
     *
     *      <tr> <td>red           <td> 0x1   <td>0-255    <td>The red channel of the RGB color.
     *      <tr> <td>green         <td> 0x2   <td>0-255    <td>The green channel of the RGB color.
     *      <tr> <td>blue          <td> 0x3   <td>0-255    <td>The blue channel of the RGB color.
     */
    class rgb_color : public attribute {
        /// The red channel of the RGB color.
        uint8_t m_red;
        /// The green channel of the RGB color.
        uint8_t m_green;
        /// The blue channel of the RGB color.
        uint8_t m_blue;

    public:
        constexpr static attribute_channel red   = 0x1;
        constexpr static attribute_channel green = 0x2;
        constexpr static attribute_channel blue  = 0x3;

        rgb_color() :
            attribute("rgb_color"),
            m_red(0),
            m_blue(0),
            m_green(0)
        {}

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
         * @brief Get the value of the red channel of the RGB color.
         *
         * @return The value of the red channel of the RGB color.
         */
        [[nodiscard]]
        uint8_t red_channel() const noexcept {
            return m_red;
        }

        /**
         * @brief Set the value of the red channel of the RGB color.
         *
         * @param a_red The value to set the red channel of the RGB color.
         */
        void set_red_channel(uint8_t const a_red) noexcept {
            m_red = a_red;
        }

        /**
         * @brief Get the value of the green channel of the RGB color.
         *
         * @return The value of the green channel of the RGB color.
         */
        [[nodiscard]]
        uint8_t green_channel() const noexcept {
            return m_green;
        }

        /**
         * @brief Set the value of the green channel of the RGB color.
         *
         * @param a_green The value to set the green channel of the RGB color.
         */
        void set_green_channel(uint8_t const a_green) noexcept {
            m_green = a_green;
        }

        /**
         * @brief Get the value of the blue channel of the RGB color.
         *
         * @return The value of the blue channel of the RGB color.
         */
        [[nodiscard]]
        uint8_t blue_channel() const noexcept {
            return m_blue;
        }

        /**
         * @brief Set the value of the blue channel of the RGB color.
         *
         * @param a_blue The value to set the blue channel of the RGB color.
         */
        void set_blue_channel(uint8_t const a_blue) noexcept {
            m_blue = a_blue;
        }

        /// Interface function to permit structured bindings.
        template <size_t v_index>
        auto get() {
            if constexpr (v_index == 0) {
                return red_channel();
            } else if constexpr (v_index == 1) {
                return green_channel();
            } else if constexpr (v_index == 2) {
                return blue_channel();
            }
        }
    };

}

// Structured bindings details.
namespace std {

    template <> struct tuple_size<monet::channel::attribute::rgb_color> : std::integral_constant<size_t, 3> {};

    template <> struct tuple_element<0, monet::channel::attribute::rgb_color> {
        using type = uint8_t;
    };

    template <> struct tuple_element<1, monet::channel::attribute::rgb_color> {
        using type = uint8_t;
    };

    template <> struct tuple_element<2, monet::channel::attribute::rgb_color> {
        using type = uint8_t;
    };

}

#endif //MASTER_SERVER_COLOR_HPP
