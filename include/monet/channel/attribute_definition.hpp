//
// Created by maxng on 12/7/2023.
//

#ifndef MASTER_SERVER_ATTRIBUTE_DEFINITION_HPP
#define MASTER_SERVER_ATTRIBUTE_DEFINITION_HPP

#include <string>
#include <map>

namespace monet::channel {

    /**
     * @brief Attribute definition for a channel configuration.
     *
     * Defines the name of an attribute and any properties it may have for a channel configuration. Can be used for
     * any type of channel attribute. Simply outlines the parameters and restrictions for an attribute but no actual
     * attribute data.
     */
    class attribute_definition {
        /// Name of the attribute in the context of a channel configuration.
        std::string m_name;
        /// Properties, including restrictions and other behavior, of the attribute.
        std::map<std::string, std::string, std::equal_to<>> m_properties;

    public:
        /**
         * @brief Create an attribute definition with a name.
         *
         * @param a_name The name of the attribute definition.
         */
        explicit attribute_definition(std::string a_name) noexcept :
            m_name(std::move(a_name))
        {}

        /**
         * @brief Create an attribute definition with a name and initial property list.
         *
         * @param a_name The name of the attribute definition.
         * @param a_properties The initial property list of the attribute definition.
         */
        attribute_definition(std::string a_name, std::map<std::string, std::string, std::equal_to<>> a_properties) noexcept :
            m_name(std::move(a_name)),
            m_properties(std::move(a_properties))
        {}

        attribute_definition(attribute_definition const& a_attribute_definition) noexcept = default;
        attribute_definition(attribute_definition&& a_attribute_definition) noexcept = default;

        attribute_definition& operator = (attribute_definition const& a_attribute_definition) noexcept = default;
        attribute_definition& operator = (attribute_definition&& a_attribute_definition) noexcept = default;

        /**
         * @brief Get the name of the attribute.
         *
         * @return The name of the attribute.
         */
        [[nodiscard]]
        std::string_view name() const noexcept {
            return m_name;
        }

        /**
         * @brief Set the name of the attribute.
         *
         * @param a_name The name of the attribute.
         */
        void set_name(std::string a_name) noexcept {
            m_name = std::move(a_name);
        }

        /**
         * @brief Get a list of properties.
         *
         * @return A list of properties.
         */
        [[nodiscard]]
        auto& properties() noexcept {
            return m_properties;
        }

        /**
         * @brief Get a list of properties.
         *
         * @return A list of properties.
         */
        [[nodiscard]]
        auto const& properties() const noexcept {
            return m_properties;
        }

        /**
         * @brief Get the value of a property.
         *
         * @param a_property_name The name of the property.
         *
         * @return The value of the property of the specified name.
         */
        [[nodiscard]]
        std::string_view property(std::string_view a_property_name) const noexcept;

        /**
         * @brief Set the value of a property.
         *
         * @param a_property_name  The name of the property to set.
         * @param a_property_value The value to which to set the property.
         */
        void set_property(std::string a_property_name, std::string a_property_value) noexcept;
    };

}

#endif //MASTER_SERVER_ATTRIBUTE_DEFINITION_HPP
