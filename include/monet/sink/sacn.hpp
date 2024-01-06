//
// Created by maxng on 1/5/2024.
//

#ifndef MASTER_SERVER_SACN_HPP
#define MASTER_SERVER_SACN_HPP

#include <memory>

#include <e131.h>

#include "../server.hpp"
#include "sink.hpp"

namespace monet::sink {
    class sacn : public sink {
        int m_socket_id;
        std::string m_source_name;
        std::unordered_map<size_t, std::unique_ptr<std::pair<e131_addr_t, e131_packet_t>>> m_multicast_packets;

    public:
        explicit sacn(std::string a_source_name = "MoNET") noexcept :
            sink("sacn"),
            m_socket_id(0),
            m_source_name(std::move(a_source_name))
        {}

        std::string_view source_name() const noexcept {
            return m_source_name;
        }

        void set_source_name(std::string a_source_name) noexcept {
            m_source_name = std::move(a_source_name);
        }

        /**
         * @brief Initialize the adapter.
         *
         * @param a_server A reference to the host server instance.
         */
        bool initialize(server& a_server) override;

        /**
         * @brief Output data to a specific universe.
         *
         * @param a_universe_number The universe number.
         * @param a_universe        The universe.
         */
        void send_universe(size_t a_universe_number, address::universe const& a_universe) override;

    private:
        /**
         * @brief Initialize a sACN packet and address for a universe.
         *
         * @param a_universe The universe number.
         */
        decltype(m_multicast_packets)::iterator initialize_universe_packet(size_t a_universe);
    };

}

#endif //MASTER_SERVER_SACN_HPP
