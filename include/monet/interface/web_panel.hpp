//
// Created by maxng on 12/2/2023.
//

#ifndef MASTER_SERVER_WEB_PANEL_HPP
#define MASTER_SERVER_WEB_PANEL_HPP

#include <string>
#include <thread>
#include <memory>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#undef interface

#include "../definitions.hpp"

namespace monet::interface {

    class web_panel {
        std::thread m_main_thread;

        std::unique_ptr<httplib::SSLServer> m_server;

        uint16_t m_port;

        std::string m_certificate_file;
        std::string m_private_key_file;

    public:
        web_panel() :
            m_main_thread(),
            m_server(nullptr),
            m_port(default_web_panel_port),
            m_certificate_file(),
            m_private_key_file()
        {}

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
         * @brief Set the SSL certificate file.
         */
        void set_ssl_certificate_file(std::string_view const a_certificate_file) noexcept {
            m_certificate_file = a_certificate_file;
        }

        /**
         * @brief Get the SSL certificate file.
         */
        [[nodiscard]]
        std::string_view ssl_certificate_file() const noexcept {
            return m_certificate_file;
        }

        /**
         * @brief Set the SSL private key file.
         */
        void set_ssl_private_key_file(std::string_view const a_private_key_file) noexcept {
            m_private_key_file = a_private_key_file;
        }

        /**
         * @brief Get the SSL private key file.
         */
        [[nodiscard]]
        std::string_view ssl_private_key_file() const noexcept {
            return m_private_key_file;
        }

        /**
         * @brief Set server port.
         */
        void set_port(uint16_t const a_port) noexcept {
            m_port = a_port;
        }

        /**
         * @brief Get server port.
         */
        [[nodiscard]]
        uint16_t port() const noexcept {
            return m_port;
        }
    };

}

#endif //MASTER_SERVER_WEB_PANEL_HPP
