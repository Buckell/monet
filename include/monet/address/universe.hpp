//
// Created by maxng on 11/13/2023.
//

#ifndef MASTER_SERVER_UNIVERSE_HPP
#define MASTER_SERVER_UNIVERSE_HPP

#include <vector>
#include <cstdint>

#include "../definitions.hpp"

namespace monet::address {

    /**
     * @brief Stores data for one universe.
     *
     * A storage object for a universe (1 byte start code + 512 byte data).
     */
    class universe {
        /// Internal storage buffer for universe data.
        std::vector<std::uint8_t> m_data;

    public:
        universe() :
            m_data()
        {
            m_data.resize(universe_buffer_size);
        }

        /// Retrieve the internal buffer of the universe data (1 byte start code + 512 byte data).
        [[nodiscard]]
        uint8_t const* buffer() const noexcept {
            return m_data.data();
        }

        /**
         * @brief Set the value for a address.
         * @param a_index The index of the address to change.
         * @param a_value The new value to which to set the address.
         *
         * Set the value for an address. An index of 0 will set the start code, and actual
         * address data starts at index 1, which will line up with the conventional 1-indexed
         * address numbers.
         */
        void set_address(size_t const a_index, uint8_t const a_value) noexcept {
            if (a_index < universe_buffer_size) [[likely]] {
                m_data[a_index] = a_value;
            }
        }

        /**
         * @brief Get the value of the address at the specified index.
         *
         * @param a_index The index of the address of which to get the value.
         *
         * @return The current value of the address or 0 if the address index is out of bounds.
         *
         * Get the value of a address by index. An index of zero will return the start code.
         */
         [[nodiscard]]
         uint8_t address(size_t const a_index) const noexcept {
             return a_index < universe_buffer_size ? m_data[a_index] : 0;
         }
    };

}

#endif //MASTER_SERVER_UNIVERSE_HPP
