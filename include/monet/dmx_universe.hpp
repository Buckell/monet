//
// Created by maxng on 11/13/2023.
//

#ifndef MASTER_SERVER_DMX_UNIVERSE_HPP
#define MASTER_SERVER_DMX_UNIVERSE_HPP

#include <vector>
#include <cstdint>

#include "definitions.hpp"

namespace monet::dmx {
    /**
     * @brief Stores data for one DMX universe.
     *
     * A storage object for a DMX universe (1 byte start code + 512 byte data).
     */
    class universe {
        /// Internal storage buffer for DMX data.
        std::vector<std::uint8_t> m_data;

    public:
        universe() : m_data() {
            m_data.resize(dmx_universe_buffer_size);
        }

        /// Retrieve the internal buffer of the DMX data (1 byte start code + 512 byte data).
        [[nodiscard]]
        uint8_t const* buffer() const noexcept {
            return m_data.data();
        }

        /**
         * @brief Set the value for a channel.
         * @param a_index The index of the channel to change.
         * @param a_value The new value to which to set the channel.
         *
         * Set the value for a channel. An index of 0 will set the start code, and actual
         * channel data starts at index 1, which will line up with the conventional 1-indexed
         * DMX channel numbers.
         */
        void set_channel(size_t const a_index, uint8_t const a_value) noexcept {
            if (a_index < dmx_universe_buffer_size) [[likely]] {
                m_data[a_index] = a_value;
            }
        }

        /**
         * @brief Get the value of the channel at the specified index.
         * @param a_index The index of the channel of which to get the value.
         *
         * @return The current value of the channel or 0 if the channel index is out of bounds.
         *
         * Get the value of a channel by index. An index of zero will return the start code.
         */
         [[nodiscard]]
         uint8_t get_channel(size_t const a_index) const noexcept {
             return a_index < dmx_universe_buffer_size ? m_data[a_index] : 0;
         }
    };
}

#endif //MASTER_SERVER_DMX_UNIVERSE_HPP
