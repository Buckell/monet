//
// Created by maxng on 12/2/2023.
//

#include <monet.hpp>

namespace monet {

    void server::start() {
        if (m_sink) {
            m_sink->initialize(*this);
        }

        m_running = true;

        m_main_thread = std::thread([&] {
            while (m_running) poll();
        });
    }

    void server::stop() {
        if (m_sink) {
            m_sink->deinitialize();
        }

        m_running = false;
        m_main_thread.join();
    }

    // Main update function.
    void server::poll() {
        auto current_time_point = std::chrono::high_resolution_clock::now();
        auto time_from_last_frame = std::chrono::nanoseconds(current_time_point - m_last_frame_time);

        // Time between last frame and now is more than calculated time between framerates.
        if (time_from_last_frame.count() >= m_sink_frame_time) {
            m_last_frame_time = current_time_point;

            if (m_sink) {
                for (auto& [universe_number, universe] : m_universes) {
                    m_sink->send_universe(universe_number, *universe);
                }
            }
        }
    }

}