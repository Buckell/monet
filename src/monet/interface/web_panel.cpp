//
// Created by maxng on 12/6/2023.
//

#include <monet.hpp>

namespace monet::interface {

    void web_panel::start() {
        setup_api_endpoints();

        m_main_thread = std::thread([&] {
            m_server = std::make_unique<httplib::SSLServer>(
                m_certificate_file.c_str(),
                m_private_key_file.c_str()
            );

            // API request.
            m_server->Get(R"(/api(.+))", [this](httplib::Request const& req, httplib::Response& res) {
                std::string const api_path{req.matches[1]};

                if (auto const it = m_api_get_handlers.find(api_path); it != m_api_get_handlers.cend()) {
                    auto const& handler = it->second;

                    if (handler.authenticate && !authenticate(req)) {
                        auto const response = nlohmann::json({
                            { "error", "Resource not found." }
                        });

                        res.status = 401;
                        return res.set_content(to_string(response), "application/json");
                    }

                    return handler.callback(req, res);
                }

                auto const response = nlohmann::json({
                    { "error", "Resource not found." }
                });

                res.status = 404;
                return res.set_content(to_string(response), "application/json");
            });

            m_server->Post(R"(/api(.+))", [this](httplib::Request const& req, httplib::Response& res) {
                std::string const api_path{req.matches[1]};

                if (auto const it = m_api_post_handlers.find(api_path); it != m_api_post_handlers.cend()) {
                    auto const& handler = it->second;

                    if (handler.authenticate && !authenticate(req)) {
                        auto const response = nlohmann::json({
                            { "error", "Resource not found." }
                        });

                        res.status = 401;
                        return res.set_content(to_string(response), "application/json");
                    }

                    return handler.callback(req, res);
                }

                auto const response = nlohmann::json({
                    { "error", "Resource not found." }
                });

                res.status = 404;
                return res.set_content(to_string(response), "application/json");
            });

            // Non-API front-end request.
            m_server->Get(R"(^(?!/api).+$)", [](httplib::Request const& req, httplib::Response& res) {
                std::string const request_path{req.matches[0]};

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

    bool web_panel::authenticate(httplib::Request const& a_request) const noexcept {
        if (!a_request.has_header("Authorization")) {
            return false;
        }

        const auto& authorization_header = a_request.get_header_value("Authorization");


        return true;
    }

    void web_panel::api_get(std::string a_path, handler a_handler) {
        m_api_get_handlers.emplace(std::move(a_path), std::move(a_handler));
    }

    void web_panel::api_post(std::string a_path, handler a_handler) {
        m_api_get_handlers.emplace(std::move(a_path), std::move(a_handler));
    }

    void web_panel::setup_api_endpoints() {
        api_get("/test", {
            .callback = [] (httplib::Request const& req, httplib::Response& res) {
                auto const response = nlohmann::json({
                    { "hello", "world" }
                });

                res.set_content(to_string(response), "application/json");
            }
        });
    }

}