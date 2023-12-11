//
// Created by maxng on 12/6/2023.
//

#ifndef MASTER_SERVER_ADDRESS_MAPPING_HPP
#define MASTER_SERVER_ADDRESS_MAPPING_HPP

#include <string>

namespace monet::channel {

    /**
     * @brief A mapping of where to select data from the attributes of a channel.
     *
     * An address mapping defines the attribute type, the index of the attribute relative to type, and the channel of
     * the attribute from which to select data. The address mapping will be tied to an address and will determine the
     * location of the data value of said address within a channel's attributes.
     */
    class address_mapping {
        /// Type of the attribute from which to retrieve data.
        std::string m_attribute_type;
        /// The index of the attribute relative to attribute type.
        size_t m_attribute_index;
        /// The attribute channel from which to select data.
        std::string m_attribute_channel;

    public:
        /**
         * @brief Constructs an address mapping.
         *
         * @param a_attribute_type    The type of the attribute from which to retrieve data.
         * @param a_attribute_index   The index of the attribute relative to attribute type.
         * @param a_attribute_channel The attribute channel from which to select data.
         */
        address_mapping(std::string a_attribute_type = "null", size_t const a_attribute_index = 0, std::string a_attribute_channel = "default") noexcept :
            m_attribute_type(std::move(a_attribute_type)),
            m_attribute_index(a_attribute_index),
            m_attribute_channel(std::move(a_attribute_channel))
        {}

        address_mapping(address_mapping const&) noexcept = default;
        address_mapping(address_mapping&&) noexcept      = default;

        address_mapping& operator = (address_mapping const&) noexcept = default;
        address_mapping& operator = (address_mapping&&) noexcept      = default;

        /**
         * @brief Get the attribute type from which to retrieve data.
         *
         * @return The attribute type from which to retrieve data.
         */
        [[nodiscard]]
        std::string_view attribute_type() const noexcept {
            return m_attribute_type;
        }

        /**
         * @brief Set the attribute type from which to retrieve data.
         */
         void set_attribute_type(std::string a_attribute_type) {
             m_attribute_type = std::move(a_attribute_type);
         }

        /**
         * @brief Get the attribute index relative to attribute type.
         *
         * @return The attribute index relative to attribute type.
         */
        [[nodiscard]]
        size_t attribute_index() const noexcept {
            return m_attribute_index;
        }

        /**
         * @brief Set the attribute index relative to attribute type.
         */
        void set_attribute_index(size_t const a_attribute_index) noexcept {
            m_attribute_index = a_attribute_index;
        }

        /**
         * @brief Get the attribute channel from which to select data.
         *
         * @return The attribute channel from which to select data.
         */
        [[nodiscard]]
        std::string_view attribute_channel() const noexcept {
            return m_attribute_channel;
        }

        /**
         * @brief Set the attribute channel from which to select data.
         */
        void set_attribute_channel(std::string a_attribute_channel) noexcept {
            m_attribute_channel = std::move(a_attribute_channel);
        }

        /// Interface function to permit structured bindings.
        template <size_t v_index>
        auto get() {
            if constexpr (v_index == 0) {
                return attribute_type();
            } else if constexpr (v_index == 1) {
                return attribute_index();
            } else if constexpr (v_index == 2) {
                return attribute_channel();
            }
        }
    };
}

// Structured bindings details.
namespace std {

    template <> struct tuple_size<monet::channel::address_mapping> : std::integral_constant<size_t, 3> {};

    template <> struct tuple_element<0, monet::channel::address_mapping> {
        using type = decltype(monet::channel::address_mapping().attribute_type());
    };

    template <> struct tuple_element<1, monet::channel::address_mapping> {
        using type = decltype(monet::channel::address_mapping().attribute_index());
    };

    template <> struct tuple_element<2, monet::channel::address_mapping> {
        using type = decltype(monet::channel::address_mapping().attribute_channel());
    };

}

#endif //MASTER_SERVER_ADDRESS_MAPPING_HPP
