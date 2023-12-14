//
// Created by maxng on 12/6/2023.
//

#ifndef MASTER_SERVER_CONFIGURATION_HPP
#define MASTER_SERVER_CONFIGURATION_HPP

#include <string>
#include <map>
#include <numeric>

#include "address_mapping.hpp"
#include "attribute_definition.hpp"

/*
 * Address Mapping:
 *
 * enable
 *     (default) - 255 for on, 0 for off
 *     bin - 1 for on, 0 for off
 *
 * intensity
 *     (default) - 0 to 255
 *     normal - 0 to 100
 *
 * rgb_color
 *     r - red channel of RGB (0-255)
 *     g - green channel of RGB (0-255)
 *     b - blue channel of RGB (0-255)
 *
 * integer
 *     (default) - 0 to 255 (clamped)
 *     b0 - least significant byte
 *     b1 - second least significant byte
 *     b(n) - nth least significant byte
 *     b8 - most significant byte
 *     (max 8 bytes, 64-bit integer)
 */

namespace monet::channel {

    /**
     * @brief Defines the structure and setup of a channel.
     *
     * Defines the name of the type of channel, the attributes a channel will have, and how the data of said attributes
     * will be mapped to addresses.
     */
    class configuration {
        /// Name of the channel configuration.
        std::string m_name;
        /// List (by attribute type) of attribute definitions.
        std::map<std::string, std::vector<attribute_definition>, std::equal_to<>> m_attribute_definitions;
        /// Mappings of attribute channels to addresses, in order. Indices are offsets to the base channel address.
        std::vector<address_mapping> m_address_mappings;

    public:
        /**
         * @brief Create a channel configuration.
         *
         * @param a_name The name of the configuration.
         */
        explicit configuration(std::string a_name) :
            m_name(std::move(a_name))
        {}

        configuration(configuration const& a_configuration) noexcept = default;
        configuration(configuration&& a_configuration) noexcept      = default;

        configuration& operator = (configuration const& a_configuration) noexcept = default;
        configuration& operator = (configuration&& a_configuration) noexcept      = default;

        /**
         * @brief Get the name of the configuration.
         *
         * @return The name of the configuration.
         */
        [[nodiscard]]
        std::string_view name() const noexcept {
            return m_name;
        }

        /**
         * @brief Get the total amount of attribute definitions.
         *
         * @return The total amount.
         */
        [[nodiscard]]
        size_t attribute_count() const noexcept;

        /**
         * @brief Get the amount of attribute definitions by type.
         *
         * @param a_attribute_type The attribute type of which to get the amount.
         *
         * @return The amount of attribute definitions of the specified type.
         */
        [[nodiscard]]
        size_t attribute_count(std::string_view a_attribute_type) const noexcept;

        /**
         * @brief Add an attribute definition.
         *
         * @param a_type                 The type of the attribute.
         * @param a_attribute_definition The attribute definition.
         */
        void add_attribute(std::string a_type, attribute_definition a_attribute_definition);

        /**
         * @brief Get all attribute definitions.
         *
         * @return All attribute definitions.
         */
        [[nodiscard]]
        auto& attributes() noexcept {
            return m_attribute_definitions;
        }

        /**
         * @brief Get all attribute definitions.
         *
         * @return All attribute definitions.
         */
        [[nodiscard]]
        auto const& attributes() const noexcept {
            return m_attribute_definitions;
        }

        /**
         * @brief Get all attribute definitions of a specific type.
         *
         * @param a_type The type of which attributes to return.
         *
         * @return The attribute definitions of the specified type.
         */
        [[nodiscard]]
        std::span<attribute_definition> attributes(std::string_view a_type) noexcept;

        /**
         * @brief Get all attribute definitions of a specific type.
         *
         * @param a_type The type of which attributes to return.
         *
         * @return The attribute definitions of the specified type.
         */
        [[nodiscard]]
        std::span<attribute_definition const> attributes(std::string_view a_type) const noexcept;

        /**
         * @brief Get all address mappings for the channel configuration.
         *
         * @return All address mappings for the channel configuration.
         */
        [[nodiscard]]
        auto& address_mappings() noexcept {
            return m_address_mappings;
        }

        /**
         * @brief Get all address mappings for the channel configuration.
         *
         * @return All address mappings for the channel configuration.
         */
        [[nodiscard]]
        auto const& address_mappings() const noexcept {
            return m_address_mappings;
        }

        /**
         * @brief Get the number of addresses a channel of this configuration will occupy.
         *
         * @return The number of addresses a channel of this configuration will occupy.
         */
        [[nodiscard]]
        size_t address_count() const noexcept {
            return m_address_mappings.size();
        }

        /**
         * @brief Get the address mapping for an address offset.
         *
         * @param a_address_index The index (address offset) of the mapping.
         *
         * @return The address mapping corresponding to the specified index.
         */
        [[nodiscard]]
        address_mapping& mapping(size_t a_address_index);

        /**
         * @brief Get the address mapping for an address offset.
         *
         * @param a_address_index The index (address offset) of the mapping.
         *
         * @return The address mapping corresponding to the specified index.
         */
        [[nodiscard]]
        address_mapping const& mapping(size_t a_address_index) const;
    };

}

#endif //MASTER_SERVER_CONFIGURATION_HPP
