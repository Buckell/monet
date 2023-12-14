//
// Created by maxng on 12/6/2023.
//

#ifndef MASTER_SERVER_ATTRIBUTE_HPP
#define MASTER_SERVER_ATTRIBUTE_HPP

#include <span>
#include <string>
#include <vector>

#include "../../definitions.hpp"

namespace monet::channel::attribute {

    /**
     * @brief Interface for different attributes.
     *
     * Allows values of attributes to be queried through a standardized interface. Attributes open up channels through
     * which different byte values can be retrieved. Channels are referenced by unsigned integers, defined respectively
     * by the attribute subclass.
     */
    class attribute {
        /// Attribute type name.
        std::string_view const m_name;

    public:
        /// Base channel ID.
        constexpr static attribute_channel base = 0x0;

    protected:
        /**
         * @brief Initialize the attribute superclass.
         *
         * @param a_name The type name of the attribute.
         */
        explicit attribute(std::string_view const a_name) : m_name(a_name) {}

    public:
        attribute(attribute const&) = delete;
        attribute(attribute&&)      = delete;

        attribute& operator = (attribute const&) = delete;
        attribute& operator = (attribute&&)      = delete;

        /**
         * @brief Get the name of the attribute.
         *
         * @return The name of the attribute.
         */
        [[nodiscard]]
        std::string_view name() noexcept {
            return m_name;
        }

        /**
         * @brief Get a list of available attribute channels by string, respective to the IDs corresponding to each
         * string ID.
         *
         * @note Number of available channels and names of available channels must correspond to the IDs of channels.
         */
         [[nodiscard]]
        virtual std::span<std::string_view const> available_channels() const noexcept = 0;

        /**
         * @brief Get the value of an attribute channel.
         *
         * @param a_channel The channel name.
         *
         * @return The value of the channel.
         */
        [[nodiscard]]
        virtual uint8_t value(attribute_channel a_channel) const noexcept = 0;

        /**
         * @brief Get the value of the default channel.
         *
         * @return The value of the default channel.
         *
         * @note Directly calls and returns value(base).
         */
        [[nodiscard]]
        uint8_t value() const noexcept {
            return value(base);
        }

        /**
         * @brief Set the value of a channel.
         *
         * @param a_channel The channel name.
         * @param a_value The value to set the channel.
         *
         * @note This function is not guaranteed to be available for all channels.
         */
        virtual void set_value(attribute_channel a_channel, uint8_t a_value) = 0;

        /**
         * @brief Get the channel ID of the corresponding channel name.
         *
         * @param a_channel The channel name from which to derive the channel ID.
         *
         * @return The channel ID corresponding to the specified channel name.
         *
         * Get the channel ID of the corresponding channel name. Defaults to 0x0 (base) if the channel name is not
         * found.
         *
         * @note Uses the values of attribute::available_channels().
         */
        [[nodiscard]]
        attribute_channel channel_name_to_id(std::string_view a_channel_name) const noexcept;

        /**
         * @brief Get the channel name of the corresponding channel ID.
         *
         * @param a_channel The channel ID corresponding to the channel name.
         *
         * @return The channel name corresponding to the specified channel ID.
         *
         * Get the channel name of the corresponding channel ID. Defaults to 0x0 (base) if the channel ID is out of
         * range.
         *
         * @note Uses the values of attribute::available_channels().
         */
        [[nodiscard]]
        std::string_view channel_id_to_name(attribute_channel a_channel_id) const noexcept;

        /**
         * @brief Check if a channel name exists.
         *
         * @param a_channel_name The channel name to check.
         *
         * @return True if the channel name exists and false if otherwise.
         *
         * @note Uses the values of attribute::available_channels().
         */
        [[nodiscard]]
        bool channel_name_exists(std::string_view a_channel_name) const noexcept;

        /**
         * @brief Check if a channel ID is in range.
         *
         * @param a_channel_name The channel ID to check.
         *
         * @return True if the channel ID is in range and false if otherwise.
         *
         * @note Uses the values of attribute::available_channels().
         */
        [[nodiscard]]
        bool channel_id_exists(attribute_channel a_channel_id) const noexcept;
    };

}

#endif //MASTER_SERVER_ATTRIBUTE_HPP
