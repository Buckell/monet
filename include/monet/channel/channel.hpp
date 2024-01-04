//
// Created by maxng on 12/13/2023.
//

#ifndef MASTER_SERVER_CHANNEL_HPP
#define MASTER_SERVER_CHANNEL_HPP

#include <unordered_map>

#include "attribute/attribute.hpp"
#include "configuration.hpp"

namespace monet::channel {

    /**
     * @brief Represents a channel with all of its attributes.
     */
    class channel {
        /// The configuration of the channel.
        configuration& m_configuration;
        /// The attributes (attribute values) of the channel.
        std::unordered_map<std::string_view, std::vector<std::unique_ptr<attribute::attribute>>> m_attributes;
        /// The base address the address mappings will use to assign attribute values to addresses.
        size_t m_base_address;

    public:
        /**
         * @brief Construct a channel based on a channel configuration.
         *
         * @param a_configuration The configuration on which to derive the channel.
         */
        explicit channel(configuration& a_configuration) noexcept :
            m_configuration(a_configuration),
            m_attributes(),
            m_base_address(0)
        {
            propagate_attributes();
        }

        channel(channel const&) = delete;
        channel(channel&&)      = delete;

        channel& operator = (channel const&) = delete;
        channel& operator = (channel&&)      = delete;

        /**
         * @brief Get the configuration of the channel.
         *
         * @return The configuration of the channel.
         */
        [[nodiscard]]
        configuration& config() noexcept {
            return m_configuration;
        }

        /**
         * @brief Get the configuration of the channel.
         *
         * @return The configuration of the channel.
         */
        [[nodiscard]]
        configuration const& config() const noexcept {
            return m_configuration;
        }

        /**
         * @brief Set the configuration of the channel.
         *
         * @param a_configuration The new configuration of the channel.
         */
        void set_config(configuration& a_configuration) noexcept {
            m_configuration = a_configuration;
            propagate_attributes();
        }

        /**
         * @brief Get the total amount of attributes.
         *
         * @return The total amount.
         */
        [[nodiscard]]
        size_t attribute_count() const noexcept;

        /**
         * @brief Get the amount of attributes by type.
         *
         * @param a_attribute_type The attribute type of which to get the amount.
         *
         * @return The amount of attribute of the specified type.
         */
        [[nodiscard]]
        size_t attribute_count(std::string_view a_attribute_type) const noexcept;

        /**
         * @brief Get all attributes.
         *
         * @return All attributes.
         */
        [[nodiscard]]
        auto& attributes() noexcept {
            return m_attributes;
        }

        /**
         * @brief Get all attributes.
         *
         * @return All attributes.
         */
        [[nodiscard]]
        auto const& attributes() const noexcept {
            return m_attributes;
        }

        /**
         * @brief Get all attributes of a specific type.
         *
         * @param a_type The type of which attributes to return.
         *
         * @return The attributes of the specified type.
         */
        [[nodiscard]]
        std::span<std::unique_ptr<attribute::attribute>> attributes(std::string_view a_type) noexcept;

        /**
         * @brief Get all attribute definitions of a specific type.
         *
         * @param a_type The type of which attributes to return.
         *
         * @return The attribute definitions of the specified type.
         */
        [[nodiscard]]
        std::span<std::unique_ptr<attribute::attribute> const> attributes(std::string_view a_type) const noexcept;

        /**
         * @brief Get the base address of the channel.
         *
         * @return The base address.
         */
        [[nodiscard]]
        size_t base_address() const noexcept {
            return m_base_address;
        }

        /**
         * @brief Set the base address of the channel.
         *
         * @param a_base_address The new base address of the channel.
         */
        void set_base_address(size_t const a_base_address) noexcept {
            m_base_address = a_base_address;
        }

        /**
         * @brief Generate attribute objects for all of the attributes defined in the configuration.
         *
         * Iterates through all of the attribute definitions in the configuration, retrieving the type and using it with
         * instantiate_attribute to dynamically construct an attribute object of said type and add it to m_attributes.
         *
         * @note Automatically called by the constructor, set_config, and similar functions.
         */
        void propagate_attributes();

        /**
         * @brief Fetch the values of all the addresses.
         *
         * @return The values of all the addresses.
         *
         * Uses the address mappings to select data from specific channels of the channel's attributes.
         */
        [[nodiscard]]
        std::vector<uint8_t> fetch_address_values() const noexcept;
    };

}

#endif //MASTER_SERVER_CHANNEL_HPP
