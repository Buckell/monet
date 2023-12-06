//
// Created by maxng on 11/13/2023.
//

#ifndef MASTER_SERVER_SERVER_HPP
#define MASTER_SERVER_SERVER_HPP

#include <memory>
#include <unordered_map>
#include <optional>
#include <thread>

#include "interface/web_panel.hpp"

namespace monet {
    class server {
        std::atomic_bool m_running;
        std::thread m_main_thread;

        std::unordered_map<size_t, std::unique_ptr<dmx::universe>> m_dmx_universes;

        interface::web_panel m_web_panel_interface;

    public:
        server() : m_running(false), m_main_thread(), m_dmx_universes(), m_web_panel_interface() {}

        server(server const&) = delete;
        server(server&&)      = delete;

        server& operator = (server const&) = delete;
        server& operator = (server&&)      = delete;

        /**
         * @brief Start the server.
         *
         * Start the server by creating a new thread and looping poll().
         */
        void start();

        /**
         * @brief Stop the server.
         *
         * Stop the running server and join the thread.
         */
        void stop();

        /**
         * @brief Tick/update the server. Should be called constantly.
         *
         * Update DMX devices, progress animations/fades, handle web control panel requests, and perform
         * other miscellaneous tasks.
         */
        void poll();

        /**
         * @brief Create, allocate, and return a new DMX universe.

         * @param a_universe_id The ID with which to create the universe.
         *
         * @return A reference to the created universe.
         *
         * Create, allocate, and return a new DMX universe with the specified universe ID.
         */
        [[nodiscard]]
        dmx::universe& create_dmx_universe(size_t a_universe_id);

        /**
         * @brief Retrieve the DMX universe with the specified ID.
         *
         * @param a_universe_id The ID of the universe to fetch.
         *
         * @return An optional containing a pointer to the universe if one exists with the ID
         *         or an empty optional if no DMX universe with the specified ID has been created.
         */
        [[nodiscard]]
        std::optional<dmx::universe*> fetch_dmx_universe(size_t a_universe_id);

        /**
         * @brief Retrieve the DMX universe with the specified ID.
         *
         * @param a_universe_id The ID of the universe to fetch.
         *
         * @return An optional containing a pointer to the universe if one exists with the ID
         *         or an empty optional if no DMX universe with the specified ID has been created.
         */
        [[nodiscard]]
        std::optional<dmx::universe const*> fetch_dmx_universe(size_t a_universe_id) const;

        /**
         * @brief Retrieve the DMX universe with the specified ID.
         *
         * @param a_universe_id The ID of the universe to get or create.
         * @return              A reference to the DMX universe with the specified ID.
         *
         * Retrieve the DMX universe with the specified ID, or create one if it does not exist.
         */
        [[nodiscard]]
        dmx::universe& get_dmx_universe(size_t a_universe_id);

        /**
         * @brief Set the value of a DMX channel.
         *
         * @param a_universe_id   The ID of the universe in which to set the DMX channel value.
         * @param a_channel_index The DMX channel number within the specified universe of which to
         *                        set the value.
         * @param a_value         The value to which to set the DMX channel.
         *
         * Set the value of a DMX channel. If a channel index of zero is supplied, the start
         * code of the universe will be set to the value.
         */
        void set_dmx_channel_value(size_t a_universe_id, size_t a_channel_index, uint8_t a_value);

        /**
         * @brief Get the value of a DMX channel.
         *
         * @param a_universe_id   The ID of the universe in which to get the DMX channel value.
         * @param a_channel_index The DMX channel number within the specified universe of which to
         *                        get the value.
         *
         * @return The value of the specified DMX channel within the specified DMX universe.
         */
        [[nodiscard]]
        uint8_t get_dmx_channel_value(size_t a_universe_id, size_t a_channel_index) const;

        /**
         * @brief Set the value of a DMX channel.
         *
         * @param a_master_id The master ID of the channel to set.
         * @param a_value     The value to which to set the DMX channel.
         *
         * Set the value of a DMX channel. If a master ID of zero is passed, nothing will happen.
         */
        void set_dmx_channel_value(size_t a_master_id, uint8_t a_value);

        /**
         * @brief Get the value of a DMX channel.
         *
         * @param a_master_id The ID of the channel to set.
         *
         * @return The value of the specified DMX channel.
         */
        [[nodiscard]]
        uint8_t get_dmx_channel_value(size_t a_master_id) const;

        /**
         * @brief Get the internal web panel interface/web server.
         *
         * @return A reference to the interface::web_panel object.
         */
         [[nodiscard]]
         interface::web_panel& web_panel_interface() noexcept {
             return m_web_panel_interface;
         }

        /**
        * @brief Get the internal web panel interface/web server.
        *
        * @return A reference to the interface::web_panel object.
        */
        [[nodiscard]]
        interface::web_panel const& web_panel_interface() const noexcept {
            return m_web_panel_interface;
        }
    };
}

#endif //MASTER_SERVER_SERVER_HPP
