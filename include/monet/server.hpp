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

        std::unordered_map<size_t, std::unique_ptr<address::universe>> m_dmx_universes;

        interface::web_panel m_web_panel_interface;

    public:
        server() :
            m_running(false),
            m_main_thread(),
            m_dmx_universes(),
            m_web_panel_interface()
        {}

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
         * @brief Create, allocate, and return a new universe.
         *
         * @param a_universe The number with which to create the universe.
         *
         * @return A reference to the created universe.
         *
         * Create, allocate, and return a new universe with the specified universe number.
         */
        [[nodiscard]]
        address::universe& create_universe(size_t a_universe);

        /**
         * @brief Retrieve the universe with the specified number.
         *
         * @param a_universe The number of the universe to fetch.
         *
         * @return An optional containing a pointer to the universe if one exists with the ID
         *         or an empty optional if no universe with the specified ID has been created.
         */
        [[nodiscard]]
        std::optional<address::universe*> fetch_universe(size_t a_universe_id);

        /**
         * @brief Retrieve the universe with the specified number.
         *
         * @param a_universe The number of the universe to fetch.
         *
         * @return An optional containing a pointer to the universe if one exists with the ID
         *         or an empty optional if no universe with the specified ID has been created.
         */
        [[nodiscard]]
        std::optional<address::universe const*> fetch_universe(size_t a_universe) const;

        /**
         * @brief Retrieve the universe with the specified number.
         *
         * @param a_universe The number of the universe to get or create.
         * @return           A reference to the universe with the specified number.
         *
         * Retrieve the universe with the specified number or create one if it does not exist.
         */
        [[nodiscard]]
        address::universe& get_universe(size_t a_universe);

        /**
         * @brief Set the value of an address.
         *
         * @param a_universe The number of the universe in which to set the address value.
         * @param a_address  The address within the specified universe of which to set the value.
         * @param a_value    The value to which to set the address.
         *
         * Set the value of an address. If an address of zero is supplied, the start
         * code of the universe will be set to the value.
         */
        void set_address_value(size_t a_universe, size_t a_address, uint8_t a_value);

        /**
         * @brief Get the value of an address.
         *
         * @param a_universe The number of the universe in which to get the address value.
         * @param a_address  The address number within the specified universe of which to
         *                   get the value.
         *
         * @return The value of the specified address within the specified universe.
         */
        [[nodiscard]]
        uint8_t get_address_value(size_t a_universe, size_t a_address) const;

        /**
         * @brief Set the value of an address.
         *
         * @param a_master_address The master address of the channel to set.
         * @param a_value          The value to which to set the address.
         *
         * Set the value of an address.
         *
         * @note If a master address of zero is passed, nothing will happen.
         */
        void set_address_value(size_t a_master_address, uint8_t a_value);

        /**
         * @brief Get the value of an address.
         *
         * @param a_master_address The master address of the value to retrieve.
         *
         * @return The value of the specified address.
         */
        [[nodiscard]]
        uint8_t get_address_value(size_t a_master_address) const;

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
