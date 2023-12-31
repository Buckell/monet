//
// Created by maxng on 12/30/2023.
//

#include <monet.hpp>
#include <gtest/gtest.h>

TEST(Addresses, MasterAddressConversion) {
    {
        size_t master_address = 0;
        auto [universe, address] = monet::address::from_master_id(master_address);
        EXPECT_EQ(universe, 0);
        EXPECT_EQ(address, 0);

        size_t new_master_address = monet::address::to_master_address(universe, address);
        EXPECT_EQ(new_master_address, master_address);
    }

    {
        size_t master_address = 1;
        auto [universe, address] = monet::address::from_master_id(master_address);
        EXPECT_EQ(universe, 1);
        EXPECT_EQ(address, 1);

        size_t new_master_address = monet::address::to_master_address(universe, address);
        EXPECT_EQ(new_master_address, master_address);
    }

    {
        size_t master_address = 128;
        auto [universe, address] = monet::address::from_master_id(master_address);
        EXPECT_EQ(universe, 1);
        EXPECT_EQ(address, 128);

        size_t new_master_address = monet::address::to_master_address(universe, address);
        EXPECT_EQ(new_master_address, master_address);
    }

    {
        size_t master_address = 512;
        auto [universe, address] = monet::address::from_master_id(master_address);
        EXPECT_EQ(universe, 1);
        EXPECT_EQ(address, 512);

        size_t new_master_address = monet::address::to_master_address(universe, address);
        EXPECT_EQ(new_master_address, master_address);
    }

    {
        size_t master_address = 513;
        auto [universe, address] = monet::address::from_master_id(master_address);
        EXPECT_EQ(universe, 2);
        EXPECT_EQ(address, 1);

        size_t new_master_address = monet::address::to_master_address(universe, address);
        EXPECT_EQ(new_master_address, master_address);
    }

    {
        size_t master_address = 1024;
        auto [universe, address] = monet::address::from_master_id(master_address);
        EXPECT_EQ(universe, 2);
        EXPECT_EQ(address, 512);

        size_t new_master_address = monet::address::to_master_address(universe, address);
        EXPECT_EQ(new_master_address, master_address);
    }

    {
        size_t master_address = 1300;
        auto [universe, address] = monet::address::from_master_id(master_address);
        EXPECT_EQ(universe, 3);
        EXPECT_EQ(address, 276);

        size_t new_master_address = monet::address::to_master_address(universe, address);
        EXPECT_EQ(new_master_address, master_address);
    }
}