//
// Created by maxng on 1/15/2024.
//

#include <monet.hpp>

namespace monet {

    channel::channel* server::channel_by_number(size_t const a_id) noexcept {
        auto const it = m_channels.find(a_id);

        return it != m_channels.cend() ? it->second.get() : nullptr;
    }

    channel::channel const* server::channel_by_number(size_t const a_id) const noexcept {
        auto const it = m_channels.find(a_id);

        return it != m_channels.cend() ? it->second.get() : nullptr;
    }

    channel::channel& server::create_channel(size_t const a_id, channel::configuration& a_configuration, size_t const a_base_address) {
        return *m_channels.emplace(a_id, std::make_unique<channel::channel>(a_configuration, this, a_base_address)).first->second;
    }

    void server::delete_channel(size_t const a_id) noexcept {
        m_channels.erase(a_id);
    }

    channel::configuration& server::channel_configuration(std::string_view const a_name) noexcept {
        if (auto const it = m_configurations.find(a_name); it != m_configurations.cend()) {
            return it->second;
        }

        return m_configurations.emplace(std::string(a_name), channel::configuration(std::string(a_name))).first->second;
    }

    channel::configuration const& server::channel_configuration(std::string_view const a_name) const noexcept {
        if (auto const it = m_configurations.find(a_name); it != m_configurations.cend()) {
            return it->second;
        }

        throw std::out_of_range("a configuration with this name does not exist");
    }

    bool server::channel_configuration_exists(std::string_view const a_name) const noexcept {
        return m_configurations.contains(a_name);
    }

}