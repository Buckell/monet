//
// Created by maxng on 12/30/2023.
//

#include <monet.hpp>
#include <gtest/gtest.h>

TEST(Channels, BasicConfiguration) {
    monet::channel::configuration led_config("LED Light");

    led_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity"));
    led_config.add_attribute("rgb_color", monet::channel::attribute_definition("Color"));

    EXPECT_EQ(led_config.attribute_count(), 2);
    EXPECT_EQ(led_config.attribute_count("intensity"), 1);
    EXPECT_EQ(led_config.attribute_count("rgb_color"), 1);

    EXPECT_EQ(led_config.attributes("intensity")[0].name(), "Intensity");
    EXPECT_EQ(led_config.attributes("rgb_color")[0].name(), "Color");

    auto& address_mappings = led_config.address_mappings();
    address_mappings.emplace_back("intensity", 0);
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "intensity");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "base");
    }
    address_mappings.emplace_back("rgb_color", 0, "red");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "red");
    }
    address_mappings.emplace_back("rgb_color", 0, "green");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "green");
    }
    address_mappings.emplace_back("rgb_color", 0, "blue");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "blue");
    }

    EXPECT_EQ(address_mappings.size(), 4);
    EXPECT_EQ(led_config.address_count(), 4);
}

TEST(Channels, AdvancedConfiguration) {
    monet::channel::configuration adv_config("Advanced Configuration");

    adv_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 1"));
    adv_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 2"));
    adv_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 3"));
    adv_config.add_attribute("rgb_color", monet::channel::attribute_definition("Color 1"));
    adv_config.add_attribute("rgb_color", monet::channel::attribute_definition("Color 2"));

    EXPECT_EQ(adv_config.attribute_count(), 5);
    EXPECT_EQ(adv_config.attribute_count("intensity"), 3);
    EXPECT_EQ(adv_config.attribute_count("rgb_color"), 2);

    EXPECT_EQ(adv_config.attributes("intensity")[0].name(), "Intensity 1");
    EXPECT_EQ(adv_config.attributes("intensity")[1].name(), "Intensity 2");
    EXPECT_EQ(adv_config.attributes("intensity")[2].name(), "Intensity 3");
    EXPECT_EQ(adv_config.attributes("rgb_color")[0].name(), "Color 1");
    EXPECT_EQ(adv_config.attributes("rgb_color")[1].name(), "Color 2");

    auto& address_mappings = adv_config.address_mappings();
    address_mappings.emplace_back("intensity", 0);
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "intensity");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "base");
    }
    address_mappings.emplace_back("rgb_color", 0, "red");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "red");
    }
    address_mappings.emplace_back("rgb_color", 0, "green");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "green");
    }
    address_mappings.emplace_back("rgb_color", 0, "blue");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "blue");
    }
    address_mappings.emplace_back("intensity", 2);
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "intensity");
        EXPECT_EQ(index, 2);
        EXPECT_EQ(channel, "base");
    }
    address_mappings.emplace_back("rgb_color", 1, "red");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 1);
        EXPECT_EQ(channel, "red");
    }
    address_mappings.emplace_back("rgb_color", 0, "green");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 0);
        EXPECT_EQ(channel, "green");
    }
    address_mappings.emplace_back("rgb_color", 1, "blue");
    {
        auto const& [type, index, channel] = address_mappings.back();
        EXPECT_EQ(type, "rgb_color");
        EXPECT_EQ(index, 1);
        EXPECT_EQ(channel, "blue");
    }

    EXPECT_EQ(address_mappings.size(), 8);
    EXPECT_EQ(adv_config.address_count(), 8);
}

TEST(Channels, AdvancedChannel) {
    monet::channel::configuration adv_config("Advanced Configuration");

    adv_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 1"));
    adv_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 2"));
    adv_config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 3"));
    adv_config.add_attribute("rgb_color", monet::channel::attribute_definition("Color 1"));
    adv_config.add_attribute("rgb_color", monet::channel::attribute_definition("Color 2"));

    auto& address_mappings = adv_config.address_mappings();
    address_mappings.emplace_back("intensity", 0);
    address_mappings.emplace_back("rgb_color", 0, "red");
    address_mappings.emplace_back("rgb_color", 0, "green");
    address_mappings.emplace_back("rgb_color", 0, "blue");
    address_mappings.emplace_back("intensity", 2, "normal");
    address_mappings.emplace_back("rgb_color", 1, "red");
    address_mappings.emplace_back("rgb_color", 0, "green");
    address_mappings.emplace_back("rgb_color", 1, "blue");

    monet::channel::channel adv_channel(adv_config);

    adv_channel.attributes("intensity")[0]->set_value(monet::channel::attribute::intensity::base, 127);
    adv_channel.attributes("intensity")[2]->set_value(monet::channel::attribute::intensity::base, 255);

    auto& rgb1 = adv_channel.attributes("rgb_color")[0];
    rgb1->set_value(monet::channel::attribute::rgb_color::red,   80);
    rgb1->set_value(monet::channel::attribute::rgb_color::green, 160);
    rgb1->set_value(monet::channel::attribute::rgb_color::blue,  240);

    auto& rgb2 = adv_channel.attributes("rgb_color")[1];
    rgb2->set_value(monet::channel::attribute::rgb_color::red,   240);
    rgb2->set_value(monet::channel::attribute::rgb_color::green, 180);
    rgb2->set_value(monet::channel::attribute::rgb_color::blue,  80);

    auto values = adv_channel.fetch_address_values();

    ASSERT_EQ(values.size(), 8);
    EXPECT_EQ(values[0], 127);
    EXPECT_EQ(values[1], 80);
    EXPECT_EQ(values[2], 160);
    EXPECT_EQ(values[3], 240);
    EXPECT_EQ(values[4], 100);
    EXPECT_EQ(values[5], 240);
    EXPECT_EQ(values[6], 160);
    EXPECT_EQ(values[7], 80);
}

TEST(Channels, AddressOutput) {
    monet::server server;
    server.create_universe(1);

    monet::channel::configuration config("Advanced Configuration");

    config.add_attribute("intensity", monet::channel::attribute_definition("Intensity 1"));
    config.add_attribute("rgb_color", monet::channel::attribute_definition("Color 1"));
    config.add_attribute("rgb_color", monet::channel::attribute_definition("Color 2"));

    auto& address_mappings = config.address_mappings();
    address_mappings.emplace_back("intensity", 0);
    address_mappings.emplace_back("rgb_color", 0, "red");
    address_mappings.emplace_back("rgb_color", 0, "green");
    address_mappings.emplace_back("rgb_color", 0, "blue");
    address_mappings.emplace_back("rgb_color", 1, "blue");
    address_mappings.emplace_back("rgb_color", 1, "green");
    address_mappings.emplace_back("rgb_color", 1, "red");

    auto& channel = server.create_channel(1, config, 1);

    channel.attributes("intensity")[0]->set_value(monet::channel::attribute::intensity::base, 127);

    auto const& rgb1 = channel.attributes("rgb_color")[0];
    rgb1->set_value(monet::channel::attribute::rgb_color::red,   80);
    rgb1->set_value(monet::channel::attribute::rgb_color::green, 160);
    rgb1->set_value(monet::channel::attribute::rgb_color::blue,  240);

    auto const& rgb2 = channel.attributes("rgb_color")[1];
    rgb2->set_value(monet::channel::attribute::rgb_color::red,   240);
    rgb2->set_value(monet::channel::attribute::rgb_color::green, 180);
    rgb2->set_value(monet::channel::attribute::rgb_color::blue,  80);

    channel.push_updates();

    auto& universe = server.get_universe(1);
    auto const* data = universe.buffer();

    EXPECT_EQ(data[1], 127);

    EXPECT_EQ(data[2], 80);
    EXPECT_EQ(data[3], 160);
    EXPECT_EQ(data[4], 240);

    EXPECT_EQ(data[5], 80);
    EXPECT_EQ(data[6], 180);
    EXPECT_EQ(data[7], 240);

    channel.set_intensity(100);

    EXPECT_EQ(data[1], 255);

    channel.set_rgb_color(255, 100, 30);

    EXPECT_EQ(data[2], 255);
    EXPECT_EQ(data[3], 100);
    EXPECT_EQ(data[4], 30);

    EXPECT_EQ(data[5], 80);
    EXPECT_EQ(data[6], 180);
    EXPECT_EQ(data[7], 240);

    channel.set_rgb_color(0x30DFAC);

    EXPECT_EQ(data[2], 48); // 0x30
    EXPECT_EQ(data[3], 223); // 0xDF
    EXPECT_EQ(data[4], 172); // 0xAC

    EXPECT_EQ(data[5], 80);
    EXPECT_EQ(data[6], 180);
    EXPECT_EQ(data[7], 240);
}