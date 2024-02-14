//
// Created by maxng on 1/15/2024.
//

#include <monet.hpp>

namespace monet {

    channel::channel* server::channel_by_number(size_t const a_id) noexcept {
        auto it = m_channels.find(a_id);

        return it != m_channels.cend() ? it->second.get() : nullptr;
    }

    channel::channel const* server::channel_by_number(size_t const a_id) const noexcept {
        auto it = m_channels.find(a_id);

        return it != m_channels.cend() ? it->second.get() : nullptr;
    }

    channel::channel& server::create_channel(size_t const a_id, channel::configuration& a_configuration, size_t const a_base_address) {
        return *m_channels.emplace(a_id, std::make_unique<channel::channel>(a_configuration, this, a_base_address)).first->second;
    }

    void server::delete_channel(size_t const a_id) noexcept {
        m_channels.erase(a_id);
    }
}