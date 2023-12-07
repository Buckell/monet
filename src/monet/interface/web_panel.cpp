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

            m_server->Get(R"(^(?!/api).+$)", [](httplib::Request const& req, httplib::Response& res) {
                std::string request_path{req.matches[0]};

                // Forward request to front-end web panel server.
                httplib::Client cli("http://localhost:3000");
                auto panel_res = cli.Get(request_path);

                res.set_content(panel_res->body, panel_res->get_header_value("Content-Type"));
            });

            // ... Setup

            m_server->listen("0.0.0.0", m_port);
        });
    }

    void web_panel::stop() {
        m_server->stop();
    }

}