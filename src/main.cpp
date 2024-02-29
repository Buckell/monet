#include <iostream>
#include <ranges>
#include <future>
#include <thread>

#include <monet.hpp>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

#include <e131.h>

std::string input() {
    std::string command;
    std::getline(std::cin, command);
    return command;
}

int main() {
    using namespace std::chrono_literals;

    monet::server server;

    auto& web_panel = server.web_panel_interface();
    web_panel.set_ssl_certificate_file("cert/ca.pem");
    web_panel.set_ssl_private_key_file("cert/ca.key");
    web_panel.start();

    server.set_sink_interface(new monet::sink::sacn());

    server.start();

    server.create_universe(1);

    auto& rgb_config = server.channel_configuration("rgb_light");
    rgb_config.add_attribute("intensity", { "Intensity", {} });
    rgb_config.add_attribute("rgb_color", { "Color", {} });

    // TODO: Add better way to emplace address mappings.
    rgb_config.address_mappings().emplace_back( "intensity" );
    rgb_config.address_mappings().emplace_back( "rgb_color", 0, "red" );
    rgb_config.address_mappings().emplace_back( "rgb_color", 0, "green" );
    rgb_config.address_mappings().emplace_back( "rgb_color", 0, "blue" );

    auto& dimmer_config = server.channel_configuration("dimmer");
    dimmer_config.add_attribute("intensity", { "Intensity", {} });
    dimmer_config.address_mappings().emplace_back("intensity", 0);

    auto& relay_config = server.channel_configuration("relay");
    relay_config.add_attribute("boolean", { "Power", {} });
    relay_config.address_mappings().emplace_back("boolean", 0);


    server.create_channel(1, "dimmer", 1); // Dimmer Channel 1
    server.create_channel(2, "dimmer", 2); // Dimmer Channel 2
    server.create_channel(3, "relay", 5);  // Shelly Channel 1
    server.create_channel(4, "relay", 27); // Remote Control Channel 1
    server.create_channel(5, "relay", 28); // Remote Control Channel 2
    server.create_channel(6, "relay", 29); // Remote Control Channel 3
    server.create_channel(7, "relay", 30); // Remote Control Channel 4
    server.create_channel(8, "relay", 31); // Remote Control Channel 5

    //chn.attributes("intensity")[0]->set_value(monet::channel::attribute::intensity::base, 0);
    //chn.push_updates();

    //std::future<std::string> input_future = std::async(std::launch::async, input);
//
//
    //
    //uint8_t value_r = 0;
    //uint8_t value_g = 0;
    //uint8_t value_b = 0;
//
    //while (true) {
    //    if (input_future.wait_for(1ms) == std::future_status::ready) {
    //        std::stringstream ss(input_future.get());
    //        std::string term;
//
    //        ss >> term;
    //        value_r = std::stoi(term);
//
    //        ss >> term;
    //        value_g = std::stoi(term);
//
    //        ss >> term;
    //        value_b = std::stoi(term);
//
    //        input_future = std::async(std::launch::async, input);
//
    //        auto& u1 = server.get_universe(1);
//
    //        auto& rgb_attr = chn.attributes("rgb_color")[0];
//
    //        rgb_attr->set_value(monet::channel::attribute::rgb_color::red, value_r);
    //        rgb_attr->set_value(monet::channel::attribute::rgb_color::green, value_g);
    //        rgb_attr->set_value(monet::channel::attribute::rgb_color::blue, value_b);
    //        chn.push_updates();
    //    }
//
    //    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    //}

    while (true);

    return EXIT_SUCCESS;

    std::vector<std::string> tab_values{
        "Control Home",
        "Settings",
        "Log"
    };
    int tab_selected = 0;
    auto tab_toggle = ftxui::Toggle(&tab_values, &tab_selected);

    auto tab_container = ftxui::Container::Tab(
        {
            ftxui::Renderer([&] {
                return ftxui::vbox({
                    ftxui::text("Hello"),
                    ftxui::filler(),
                });
            }),
            ftxui::Renderer([&] {
                return ftxui::text("Goodbye");
            }),
            ftxui::Renderer([&] {
                return ftxui::text("Hello");
            }),
        },
        &tab_selected
    );

    auto tab_combo = ftxui::Container::Vertical({
        tab_toggle,
        tab_container
    });

    auto renderer = Renderer(tab_combo, [&] {
        return ftxui::vbox({
            tab_toggle->Render(),
            ftxui::separator(),
            tab_container->Render()
        }) | ftxui::border | ftxui::color(ftxui::Color::White);
    });

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    screen.Loop(renderer);

    return EXIT_SUCCESS;
}
