//
// Created by maxng on 11/15/2023.
//

#include <monet.hpp>

namespace monet {

    dmx::universe& server::create_dmx_universe(size_t const a_universe_id) {
        auto const it = m_dmx_universes.emplace(a_universe_id, std::make_unique<dmx::universe>());
        return *it.first->second;
    }

    std::optional<dmx::universe*> server::fetch_dmx_universe(size_t const a_universe_id) {
        auto const it = m_dmx_universes.find(a_universe_id);

        if (it == m_dmx_universes.cend()) {
            return std::nullopt;
        } else {
            return std::make_optional(it->second.get());
        }
    }

    std::optional<dmx::universe const*> server::fetch_dmx_universe(size_t const a_universe_id) const {
        auto const it = m_dmx_universes.find(a_universe_id);

        if (it == m_dmx_universes.cend()) {
            return std::nullopt;
        } else {
            return std::make_optional(it->second.get());
        }
    }

    dmx::universe& server::get_dmx_universe(size_t const a_universe_id) {
        auto const it = m_dmx_universes.find(a_universe_id);

        // Return DMX universe if found or create another if not.
        if (it == m_dmx_universes.cend()) {
            return create_dmx_universe(a_universe_id);
        } else {
            return *it->second;
        }
    }

    void server::set_dmx_channel_value(size_t const a_universe_id, size_t const a_channel_index, uint8_t const a_value) {
        get_dmx_universe(a_universe_id).set_channel(a_channel_index, a_value);
    }

    uint8_t server::get_dmx_channel_value(size_t const a_universe_id, size_t const a_channel_index) const {
        auto universe_opt = fetch_dmx_universe(a_universe_id);
        return universe_opt.has_value() ? universe_opt.value()->get_channel(a_channel_index) : 0;
    }

    void server::set_dmx_channel_value(size_t const a_master_id, uint8_t const a_value) {
        // Only complete on valid master ID.
        if (a_master_id > 0) [[likely]] {
            auto const [universe_id, channel_id] = dmx::from_master_id(a_master_id);
            get_dmx_universe(universe_id).set_channel(channel_id, a_value);
        }
    }

    uint8_t server::get_dmx_channel_value(size_t const a_master_id) const {
        // Only complete on valid master ID.
        if (a_master_id > 0) [[likely]] {
            auto const [universe_id, channel_id] = dmx::from_master_id(a_master_id);
            auto universe_opt = fetch_dmx_universe(universe_id);
            return universe_opt.has_value() ? universe_opt.value()->get_channel(channel_id) : 0;
        } else {
            return 0;
        }
    }

}