//
// Created by maxng on 12/2/2023.
//

#ifndef MASTER_SERVER_WEB_PANEL_HPP
#define MASTER_SERVER_WEB_PANEL_HPP

#include <string>
#include <thread>

namespace monet::interface {
    class web_panel {
        std::thread m_main_thread;

        std::string m_certificate_file;
        std::string m_private_key_file;

    public:
        web_panel() : m_main_thread(), m_certificate_file(), m_private_key_file() {}

        web_panel(web_panel const&) = delete;
        web_panel(web_panel&&)      = delete;

        web_panel& operator = (web_panel const&) = delete;
        web_panel& operator = (web_panel&&)      = delete;

        /**
         * @brief Start the web panel server.
         *
         * Creates a new thread for the blocking web server and returns.
         */
        void start();

        /**
         * @brief Stop the web panel server.
         *
         * Stops the server and joins the thread.
         */
        void stop();

        /**
         *
         */
    };
}

#endif //MASTER_SERVER_WEB_PANEL_HPP
