//
// Created by maxng on 11/15/2023.
//

#ifndef MASTER_SERVER_MASTER_ID_HPP
#define MASTER_SERVER_MASTER_ID_HPP

#include <tuple>

#include "monet/definitions.hpp"

namespace monet::dmx {

    /**
     * @brief Convert a DMX master ID to its respective universe and channel IDs.
     *
     * @param a_master_id The master ID to convert.
     *
     * @return A tuple containing two values, the universe ID in the first position [0]
     *         and the channel ID in the second position [1].
     */
    constexpr std::tuple<size_t, size_t> from_master_id(size_t const a_master_id) noexcept {
        if (a_master_id == 0) [[unlikely]] { // Null ID.
            return {0, 0};
        }

        // Adjust for 1-based indexing, divide by channel count per universe,
        // and floor (automatic with integer division).
        auto const universe_id = std::max(a_master_id - 1, 0ull) / dmx_data_channel_count;

        // Determine remainder.
        auto const channel_id = a_master_id - universe_id * dmx_data_channel_count;

        return {universe_id, channel_id};
    }

    /**
     * @brief Converts a DMX universe ID and a channel ID to a master ID.
     *
     * @param a_universe_id The universe ID to convert.
     * @param a_channel_id  The channel ID within the specified universe to convert.
     *
     * @return The converted master ID.
     */
    constexpr size_t to_master_id(size_t const a_universe_id, size_t const a_channel_id) noexcept {
        return a_universe_id * dmx_data_channel_count + a_channel_id;
    }

}

#endif //MASTER_SERVER_MASTER_ID_HPP
