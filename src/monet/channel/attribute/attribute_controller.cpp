//
// Created by maxng on 12/13/2023.
//

#include <iostream>

#include <monet.hpp>

namespace monet::channel::attribute {

    std::map<
        std::string_view,
        registry::attribute_instantiation_callback
    >& registry::exchange_instantiation_callbacks() noexcept {
        /*
         * A regular global/static definition won't work for whatever reason with all compilers,
         * so we'll put it into a function with a static definition to make sure it's initialized.
         */
        static std::map<
                std::string_view,
                attribute_instantiation_callback
        > instantiation_callbacks;

        return instantiation_callbacks;
    }

}