//
// Created by maxng on 1/5/2024.
//

#include <monet.hpp>

namespace monet::sink {

    bool sacn::initialize(server& a_server) {
        if ((m_socket_id = e131_socket()) < 0) {
            return false;
        }

        if (e131_multicast_iface(m_socket_id, 0) < 0) {
            return false;
        }
    }

    void sacn::deinitialize() {
        // libE131 doesn't seem to offer a function to close the socket. That's probably an issue, but there
        // should only be a problem when stopping the server, which will usually only happen upon program exit,
        // in which case it won't matter.
    }

    void sacn::send_universe(size_t const a_universe_number, address::universe const& a_universe) {
        auto it = m_multicast_packets.find(a_universe_number);

        if (it == m_multicast_packets.cend()) {
            it = initialize_universe_packet(a_universe_number);
        }

        auto& [addr, packet] = *it->second;

        memcpy(&packet.dmp.prop_val + 1, a_universe.buffer(), a_universe.address_count());

        e131_send(m_socket_id, &packet, &addr);

        ++packet.frame.seq_number;
    }

    decltype(sacn::m_multicast_packets)::iterator sacn::initialize_universe_packet(size_t const a_universe) {
        auto it = m_multicast_packets.emplace(
            a_universe,
            std::make_unique<std::pair<e131_addr_t, e131_packet_t>>()
        ).first;

        auto& [addr, packet] = *it->second;

        e131_pkt_init(&packet, 1, 512);
        std::memcpy(
            &packet.frame.source_name,
            m_source_name.data(),
            std::max(
                m_source_name.length() + 1,
                sizeof(packet.frame.source_name)
            )
        );

        e131_multicast_dest(
            &addr,
            1,
            E131_DEFAULT_PORT
        );

        return it;
    }

}