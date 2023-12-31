//
// Created by maxng on 12/30/2023.
//

#include <monet.hpp>
#include <gtest/gtest.h>

TEST(Attributes, Intensity) {
    using namespace monet::channel::attribute;

    auto intensity_attr = registry::instantiate_attribute("intensity");

    ASSERT_NE(intensity_attr.get(), nullptr);

    intensity_attr->set_value(intensity::normal, 50);

    EXPECT_EQ(intensity_attr->value(intensity::base),   127);
    EXPECT_EQ(intensity_attr->value(intensity::normal), 50 );

    auto* intens = dynamic_cast<intensity*>(intensity_attr.get());

    EXPECT_EQ(intens->intensity_value(), 127);

    intens->set_intensity_value(200);

    EXPECT_EQ(intens->intensity_value(),                200);
    EXPECT_EQ(intensity_attr->value(intensity::base),   200);
    EXPECT_EQ(intensity_attr->value(intensity::normal), 79 );

    intensity_attr->set_value(intensity::normal,        100);
    EXPECT_EQ(intensity_attr->value(intensity::base),   255);
}

TEST(Attributes, RGBColor) {
    using namespace monet::channel::attribute;

    auto color_attr = registry::instantiate_attribute("rgb_color");

    ASSERT_NE(color_attr.get(), nullptr);

    color_attr->set_value(rgb_color::red,   225);
    color_attr->set_value(rgb_color::green, 110);
    color_attr->set_value(rgb_color::blue,  20 );

    EXPECT_EQ(color_attr->value(rgb_color::red),   225);
    EXPECT_EQ(color_attr->value(rgb_color::green), 110);
    EXPECT_EQ(color_attr->value(rgb_color::blue),  20 );

    auto* color = dynamic_cast<rgb_color*>(color_attr.get());

    EXPECT_EQ(color->red_channel(),   225);
    EXPECT_EQ(color->green_channel(), 110);
    EXPECT_EQ(color->blue_channel(),  20 );

    color->set_red_channel(110);
    color->set_green_channel(225);
    color->set_blue_channel(20);

    EXPECT_EQ(color_attr->value(rgb_color::red),   110);
    EXPECT_EQ(color_attr->value(rgb_color::green), 225);
    EXPECT_EQ(color_attr->value(rgb_color::blue),  20 );
}