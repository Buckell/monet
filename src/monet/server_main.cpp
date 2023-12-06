//
// Created by maxng on 12/2/2023.
//

#include <monet.hpp>

namespace monet {

    void server::start() {
        m_running = true;

        m_main_thread = std::thread([&] {
            while (m_running) poll();
        });
    }

    void server::stop() {
        m_running = false;
        m_main_thread.join();
    }

    // Main update function.
    void server::poll() {

    }

}