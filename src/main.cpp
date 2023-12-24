#include <iostream>

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

int main() {
    monet::server server;

    auto& web_panel = server.web_panel_interface();
    web_panel.set_ssl_certificate_file("cert/ca.pem");
    web_panel.set_ssl_private_key_file("cert/ca.key");
    web_panel.start();

    using namespace monet::channel::attribute;

    auto intensity_attr = registry::instantiate_attribute("intensity");


    std::cout << bool(intensity_attr) << std::endl;

    intensity_attr->set_value(intensity::base, 100);
    dynamic_cast<intensity*>(intensity_attr.get())->set_intensity_value(100);
    std::cout << static_cast<size_t>(intensity_attr->value(intensity::normal)) << std::endl;


    monet::channel::configuration led_config("LED Light");

    led_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity"));
    led_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 2"));
    led_config.add_attribute("rgb_color", monet::channel::attribute_definition("Color"));

    auto& address_mappings = led_config.address_mappings();
    address_mappings.emplace_back("intensity", 0);
    address_mappings.emplace_back("intensity", 1);
    address_mappings.emplace_back("rgb_color", 0, "red");
    address_mappings.emplace_back("rgb_color", 0, "green");
    address_mappings.emplace_back("rgb_color", 0, "blue");

    monet::channel::channel led_channel(led_config);

    led_channel.attributes("intensity")[0]->set_value(monet::channel::attribute::intensity::normal, 50);
    led_channel.attributes("intensity")[1]->set_value(monet::channel::attribute::intensity::normal, 75);

    auto& color_attr = led_channel.attributes("rgb_color")[0];
    color_attr->set_value(monet::channel::attribute::rgb_color::red,   255);
    color_attr->set_value(monet::channel::attribute::rgb_color::green, 127);
    color_attr->set_value(monet::channel::attribute::rgb_color::blue,  0);

    auto address_values = led_channel.fetch_address_values();

    for (auto const value : address_values) {
        std::cout << static_cast<size_t>(value) << " ";
    }
    std::cout << std::endl;

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
