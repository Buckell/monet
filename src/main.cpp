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

    server.set_dmx_channel_value(0, 1, 10);

    httplib::SSLServer svr("cert/ca.pem", "cert/ca.key");

    svr.Get("/", [](httplib::Request const& req, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    svr.listen("0.0.0.0", 8080);

    //std::vector<std::string> tab_values{
    //    "Control Home",
    //    "Settings",
    //    "Log"
    //};
    //int tab_selected = 0;
    //auto tab_toggle = ftxui::Toggle(&tab_values, &tab_selected);
//
    //auto tab_container = ftxui::Container::Tab(
    //    {
    //        ftxui::Renderer([&] {
    //            return ftxui::vbox({
    //                ftxui::text("Hello"),
    //                ftxui::filler(),
    //            });
    //        }),
    //        ftxui::Renderer([&] {
    //            return ftxui::text("Goodbye");
    //        }),
    //        ftxui::Renderer([&] {
    //            return ftxui::text("Hello");
    //        }),
    //    },
    //    &tab_selected
    //);
//
    //auto tab_combo = ftxui::Container::Vertical({
    //    tab_toggle,
    //    tab_container
    //});
//
    //auto renderer = Renderer(tab_combo, [&] {
    //    return ftxui::vbox({
    //        tab_toggle->Render(),
    //        ftxui::separator(),
    //        tab_container->Render()
    //    }) | ftxui::border;
    //});
//
    //auto screen = ftxui::ScreenInteractive::Fullscreen();
    //screen.Loop(renderer);
//
    //return EXIT_SUCCESS;
}
