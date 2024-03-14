//
// Created by maxng on 11/15/2023.
//

#ifndef MASTER_SERVER_MASTER_ID_HPP
#define MASTER_SERVER_MASTER_ID_HPP

#include <tuple>
#include <cmath>

#include "../definitions.hpp"

namespace monet::address {

    /**
     * @brief Convert a master address to its respective universe and channel numbers.
     *
     * @param a_master_address The master address to convert.
     *
     * @return A tuple containing two values, the universe ID in the first position [0]
     *         and the channel ID in the second position [1].
     */
    constexpr std::tuple<size_t, size_t> from_master_id(size_t const a_master_address) noexcept {
        if (a_master_address == 0) [[unlikely]] { // Null ID.
            return {0, 0};
        }

        // Adjust for one-based indexing, divide by channel count per universe,
        // floor (automatic with integer division), and increase by one for one-base indexing.
        auto const universe = (std::max(a_master_address - 1, 0ull) / dmx_data_channel_count);

        // Determine remainder.
        auto const address = a_master_address - universe * dmx_data_channel_count;

        return { universe + 1, address };
    }

    /**
     * @brief Converts a universe and address number to a master address.
     *
     * @param a_universe The universe number to convert.
     * @param a_channel  The channel number within the specified universe to convert.
     *
     * @return The converted master address.
     */
    constexpr size_t to_master_address(size_t const a_universe, size_t const a_channel) noexcept {
        if (a_universe == 0 || a_channel == 0) [[unlikely]] { // Null ID.
            return 0;
        }

        return (a_universe - 1) * dmx_data_channel_count + a_channel;
    }

}

#endif //MASTER_SERVER_MASTER_ID_HPP
