//
// Created by maxng on 12/13/2023.
//

#ifndef MASTER_SERVER_ATTRIBUTE_CONTROLLER_HPP
#define MASTER_SERVER_ATTRIBUTE_CONTROLLER_HPP

#include <memory>
#include <map>
#include <type_traits>
#include <iostream>

#include "attribute.hpp"
#include "../../utility.hpp"

namespace monet::channel::attribute {

    /// A class that implements/inherits the attribute interface class.
    template <typename t_class>
    concept attribute_class = std::is_base_of_v<attribute, t_class>;

    /**
     * @brief A class to manage ambiguous constructor functions for attribute implementations.
     *
     * Maintains a registry of attribute implementations. Stores a static map of ambiguous constructor interface
     * functions indexed by the attribute ID. Allows dynamic instantiation of unique attribute implementations by a
     * string ID.
     */
    class registry {
    public:
        /// A callback to an ambiguous attribute constructor function that returns the attribute interface class.
        using attribute_instantiation_callback = std::unique_ptr<attribute> (*)();

    private:
        /// Retrieve static callback map.
        static std::map<
                std::string_view,
                attribute_instantiation_callback
        >& exchange_instantiation_callbacks() noexcept;

    public:
        /**
         * @brief Instantiates a specific attribute class and returns it as an abstracted interface object.
         *
         * @param a_attribute_name The name of the attribute of which to instantiate a class.
         *
         * @return A pointer to the abstracted attribute interface object.
         *
         * @note If the name provided does not match any registered attribute class, a null pointer will be returned.
         */
        static std::unique_ptr<attribute> instantiate_attribute(std::string_view const a_attribute_name) {
            auto& instantiation_callbacks = exchange_instantiation_callbacks();

            auto it = instantiation_callbacks.find(a_attribute_name);

            if (it == instantiation_callbacks.cend()) {
                return nullptr;
            }

            return it->second();
        }

        /**
         * @brief Dynamic registry class for registering different attribute types.
         *
         * @tparam v_attribute_name  The ID/working name of the attribute.
         * @tparam t_attribute_class The class of the attribute.
         */
        template <string_literal v_attribute_name, attribute_class t_attribute_class>
        class register_attribute {
        public:
            register_attribute() {
                auto& instantiation_callbacks = exchange_instantiation_callbacks();

                instantiation_callbacks.emplace(
                    v_attribute_name.to_string_view(),
                    // Lambda to dynamically instantiate, cast, and return the registered class as the polymorphic 'attribute'.
                    []() -> std::unique_ptr<attribute> {
                        return std::unique_ptr<attribute>(
                            dynamic_cast<attribute*>(new t_attribute_class)
                        );
                    }
                );
            }
        };
    };

}

#endif //MASTER_SERVER_ATTRIBUTE_CONTROLLER_HPP
