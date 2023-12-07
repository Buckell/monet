//
// Created by maxng on 12/6/2023.
//

#include <monet.hpp>

namespace monet::interface {

    void web_panel::start() {
        m_main_thread = std::thread([&] {
            m_server = std::make_unique<httplib::SSLServer>(
                m_certificate_file.c_str(),
                m_private_key_file.c_str()
            );

            m_server->Get("/test", [](httplib::Request const& req, httplib::Response& res) {
                res.set_content("Hello, world!", "text/plain");
            });

            // ... Setup

            m_server->listen("0.0.0.0", m_port);
        });
    }

    void web_panel::stop() {
        m_server->stop();
    }

}