//
// Created by maxng on 11/15/2023.
//

#include <monet.hpp>

namespace monet {

    address::universe& server::create_universe(size_t const a_universe) {
        auto const it = m_universes.emplace(a_universe, std::make_unique<address::universe>());
        return *it.first->second;
    }

    std::optional<address::universe*> server::fetch_universe(size_t const a_universe_id) {
        auto const it = m_universes.find(a_universe_id);

        if (it == m_universes.cend()) {
            return std::nullopt;
        } else {
            return std::make_optional(it->second.get());
        }
    }

    std::optional<address::universe const*> server::fetch_universe(size_t const a_universe) const {
        auto const it = m_universes.find(a_universe);

        if (it == m_universes.cend()) {
            return std::nullopt;
        } else {
            return std::make_optional(it->second.get());
        }
    }

    address::universe& server::get_universe(size_t const a_universe) {
        auto const it = m_universes.find(a_universe);

        // Return DMX universe if found or create another if not.
        if (it == m_universes.cend()) {
            return create_universe(a_universe);
        } else {
            return *it->second;
        }
    }

    void server::set_address_value(size_t const a_universe, size_t const a_address, uint8_t const a_value) {
        get_universe(a_universe).set_address(a_address, a_value);
    }

    uint8_t server::get_address_value(size_t const a_universe, size_t const a_address) const {
        auto universe_opt = fetch_universe(a_universe);
        return universe_opt.has_value() ? universe_opt.value()->address(a_address) : 0;
    }

    void server::set_address_value(size_t const a_master_address, uint8_t const a_value) {
        // Only complete on valid master ID.
        if (a_master_address > 0) [[likely]] {
            auto const [universe_id, channel_id] = address::from_master_id(a_master_address);
            get_universe(universe_id).set_address(channel_id, a_value);
        }
    }

    uint8_t server::get_address_value(size_t const a_master_address) const {
        // Only complete on valid master ID.
        if (a_master_address > 0) [[likely]] {
            auto const [universe_id, channel_id] = address::from_master_id(a_master_address);
            auto universe_opt = fetch_universe(universe_id);
            return universe_opt.has_value() ? universe_opt.value()->address(channel_id) : 0;
        } else {
            return 0;
        }
    }

}