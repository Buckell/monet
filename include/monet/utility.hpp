//
// Created by maxng on 12/13/2023.
//

#ifndef MASTER_SERVER_UTILITY_HPP
#define MASTER_SERVER_UTILITY_HPP

#include <cstdint>
#include <algorithm>

namespace monet {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"

    /// Helper struct for string literal template parameters.
    template <size_t v_size>
    struct string_literal {
        constexpr string_literal(const char (&a_string)[v_size]) {
            std::copy_n(a_string, v_size, value);
        }

        char value[v_size]{};
        size_t length = v_size - 1;

        [[nodiscard]]
        std::string to_string() const noexcept {
            return {value, length};
        }

        [[nodiscard]]
        std::string_view to_string_view() const noexcept {
            return {value, length};
        }
    };

#pragma clang diagnostic pop

}

#endif //MASTER_SERVER_UTILITY_HPP
