//
// Created by maxng on 12/6/2023.
//

#include <sstream>

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
                            { "error", "Not authenticated." }
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
                            { "error", "Not authenticated." }
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

                if (auto panel_res = cli.Get(request_path); panel_res) {
                    res.set_content(panel_res->body, panel_res->get_header_value("Content-Type"));
                } else {
                    res.status = 404;
                    res.set_content("Resource not found.", "text/html");
                }
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

        // TODO: Authorize header.

        return true;
    }

    void web_panel::api_get(std::string a_path, handler a_handler) {
        m_api_get_handlers.emplace(std::move(a_path), std::move(a_handler));
    }

    void web_panel::api_post(std::string a_path, handler a_handler) {
        m_api_post_handlers.emplace(std::move(a_path), std::move(a_handler));
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

        api_get("/channels/configurations", {
            .callback = [this] (httplib::Request const& req, httplib::Response& res) {
                auto response = nlohmann::json();

                for (auto& [name, configuration] : m_host.channel_configurations()) {
                    auto attributes_data = nlohmann::json();
                    auto address_mappings_data = nlohmann::json::array();

                    for (auto const & [attribute_type, attributes] : configuration->attributes()) {
                        auto attributes_list = nlohmann::json::array();

                        for (auto const& attribute : attributes) {
                            auto properties_data = nlohmann::json::array();

                            for (auto const& [property_key, property_value] : attribute.properties()) {
                                properties_data[property_key] = property_value;
                            }

                            attributes_list.push_back(nlohmann::json({
                                { "name", attribute.name() },
                                { "properties", properties_data }
                            }));
                        }

                        attributes_data[attribute_type] = attributes_list;
                    }

                    for (auto const& [type, index, channel] : configuration->address_mappings()) {
                        // address_mappings_data.push_back(nlohmann::json::array({
                        //     type,
                        //     index,
                        //     channel
                        // }));

                        std::stringstream sstr;
                        sstr << type;
                        sstr << " ";
                        sstr << index;
                        sstr << " ";
                        sstr << channel;

                        address_mappings_data.push_back(sstr.str());
                    }

                    response[configuration->name()] = nlohmann::json({
                        { "attributes", attributes_data },
                        { "address_mappings", address_mappings_data }
                    });
                }

                res.set_content(to_string(response), "application/json");
            }
        });

        api_get("/channels", {
            .callback = [this] (httplib::Request const& req, httplib::Response& res) {
                auto channels_data = nlohmann::json();

                for (auto& [index, channel] : m_host.channels()) {
                    auto attributes_data = nlohmann::json();

                    for (auto const& [attribute_type, attributes] : channel->attributes()) {
                        auto attribute_type_data = nlohmann::json::array();

                        for (int i = 0; i < attributes.size(); ++i) {
                            auto const& attribute = *attributes[i];

                            auto attribute_channel_data = nlohmann::json();

                            auto const& available_channels = attribute.available_channels();

                            for (int ii = 0; ii < available_channels.size(); ++ii) {
                                attribute_channel_data[available_channels[ii]] = attribute.value(ii);
                            }

                            attribute_type_data.push_back(nlohmann::json({
                                { "name", channel->config().attributes(attribute_type)[i].name() },
                                { "channels", attribute_channel_data }
                            }));
                        }

                        attributes_data[attribute_type] = attribute_type_data;
                    }

                    channels_data[index] = nlohmann::json({
                        { "base_address", channel->base_address() },
                        { "configuration", channel->config().name() },
                        { "attributes", attributes_data },
                        { "addresses", channel->fetch_address_values() }
                    });
                }

                res.set_content(to_string(channels_data), "application/json");
            }
        });

        api_post("/channel", {
            .callback = [this] (httplib::Request const& req, httplib::Response& res) {
                auto request_data = nlohmann::json::parse(req.body);

                auto const channel_number = request_data["channel"].get<size_t>();

                auto* channel = m_host.channel_by_number(channel_number);

                if (auto const& attributes_field = request_data["attributes"]; attributes_field.is_object()) {
                    for (auto const& [attribute_type, attribute_list] : request_data["attributes"].items()) {
                        auto const& channel_attributes = channel->attributes(attribute_type);

                        for (size_t attribute_index = 0; attribute_index < attribute_list.size(); ++attribute_index) {
                            auto const& attribute_data = attribute_list[attribute_index];
                            auto const& channel_attribute = channel_attributes[attribute_index];

                            if (attribute_data.is_null()) {
                                continue;
                            }

                            for (auto const& [attribute_channel, attribute_channel_value] : attribute_data.items()) {
                                channel_attribute->set_value(channel_attribute->channel_name_to_id(attribute_channel), attribute_channel_value.get<size_t>());
                            }
                        }
                    }
                }

                channel->push_updates();
            }
        });
    }

}