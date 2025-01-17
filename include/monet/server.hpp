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
#include "sink/sink.hpp"

namespace monet {

    class server {
        std::atomic_bool m_running;
        std::thread m_main_thread;

        std::unordered_map<size_t, std::unique_ptr<address::universe>> m_universes;
        std::unique_ptr<sink::sink> m_sink;
        size_t m_sink_framerate;
        size_t m_sink_frame_time;
        std::chrono::high_resolution_clock::time_point m_last_frame_time;

        interface::web_panel m_web_panel_interface;

        std::map<std::string_view, std::unique_ptr<channel::configuration>> m_configurations;
        std::unordered_map<size_t, std::unique_ptr<channel::channel>> m_channels;

    public:
        server() :
                m_running(false),
                m_main_thread(),
                m_universes(),
                m_sink(nullptr),
                m_sink_framerate(default_sink_framerate),
                m_sink_frame_time(std::nano::den / default_sink_framerate),
                m_last_frame_time(std::chrono::high_resolution_clock::now()),
                m_web_panel_interface(*this)
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
        address::universe& create_universe(size_t a_universe);

        /**
         * @brief Retrieve the universe with the specified number.
         *
         * @param a_universe_id The number of the universe to fetch.
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

        /**
         * @brief Get the current sink interface.
         *
         * @return A pointer to the sink interface.
         */
        [[nodiscard]]
        sink::sink* sink_interface() noexcept {
            return m_sink.get();
        }

        /**
         * @brief Get the current sink interface.
         *
         * @return A pointer to the sink interface.
         */
        [[nodiscard]]
        sink::sink const* sink_interface() const noexcept {
            return m_sink.get();
        }

        /**
         * @brief Set the sink interface.
         *
         * @param a_sink The sink interface.
         *
         * @note The current sink's lifetime is managed by the server. There is no need to
         * explicitly delete or otherwise explicitly manage its lifetime. Do not delete it
         * while it is in use by the server.
         */
        void set_sink_interface(sink::sink* a_sink) noexcept {
            if (m_running) {
                a_sink->initialize(*this);
            }

            m_sink = std::unique_ptr<sink::sink>(a_sink);
        }

        /**
         * @brief Get the framerate of the sink updating.
         *
         * @return The sink framerate.
         */
        [[nodiscard]]
        size_t sink_framerate() const noexcept {
            return m_sink_framerate;
        }

        /**
         * @brief Set the framerate of the sink updating.
         *
         * @param a_framerate The new sink framerate.
         */
        void set_sink_framerate(size_t const a_framerate) noexcept {
            m_sink_frame_time = std::nano::den / a_framerate;
            m_sink_framerate = a_framerate;
        }

        /**
         * @brief Get the channel configuration by the given name or create one if it does not exist.
         *
         * @param a_name The name of the channel configuration.
         *
         * @return A reference to the channel configuration.
         */
        channel::configuration& channel_configuration(std::string_view a_name) noexcept;

        /**
         * @brief Get the channel configuration by the given name.
         *
         * @param a_name The name of the channel configuration.
         *
         * @return A reference to the channel configuration.
         *
         * @note Throws an error if the configuration does not exist.
         */
        channel::configuration const& channel_configuration(std::string_view a_name) const noexcept;

        /**
         * @brief Checks if a channel configuration with the given name exists.
         *
         * @return Whether the configuration exists.
         */
        bool channel_configuration_exists(std::string_view a_name) const noexcept;

        /**
         * @brief Get a list of channel configurations.
         *
         * @return The list of channel configurations.
         */
        auto& channel_configurations() noexcept {
            return m_configurations;
        }

        /**
         * @brief Get a channel by its ID.
         *
         * @param a_id The channel ID.
         *
         * @return A pointer to the channel with the specified ID or nullptr if no channel is
         *         connected to the given ID.
         */
        [[nodiscard]]
        channel::channel* channel_by_number(size_t a_id) noexcept;

        /**
         * @brief Get a channel by its ID.
         *
         * @param a_id The channel ID.
         *
         * @return A pointer to the channel with the specified ID or nullptr if no channel is
         *         connected to the given ID.
         */
        [[nodiscard]]
        channel::channel const* channel_by_number(size_t a_id) const noexcept;

        /**
         * @brief Create a new channel with the given ID and configuration.
         *
         * @param a_id            The ID of the new channel.
         * @param a_configuration The name of the configuration with which to create the new channel.
         * @param a_base_address  The base address to which the address values of the channel will be mapped.
         *
         * @return The newly created channel.
         *
         * @note If a channel with the supplied ID already exists, it will be overwritten.
         */
        channel::channel& create_channel(size_t a_id, std::string_view a_configuration, size_t a_base_address = 0);

        /**
         * @brief Delete a channel by its ID.
         *
         * @param a_id The ID of the channel to delete.
         */
        void delete_channel(size_t a_id) noexcept;

        /**
         * @brief Get a list of channels.
         *
         * @return The list of channels.
         */
        auto& channels() noexcept {
            return m_channels;
        }

        /**
         * @brief Get a list of channels.
         *
         * @return The list of channels.
         */
        auto const& channels() const noexcept {
            return m_channels;
        }
    };

}

#endif //MASTER_SERVER_SERVER_HPP
