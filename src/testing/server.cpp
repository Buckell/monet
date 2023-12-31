//
// Created by maxng on 12/30/2023.
//

#include <monet.hpp>
#include <gtest/gtest.h>

class Server : public testing::Test {
protected:
    monet::server m_server;
};

TEST_F(Server, Universes) {
    // Create sample universes.
    m_server.create_universe(1);
    m_server.create_universe(7);

    {
        // Test universe instantiation.
        ASSERT_TRUE (m_server.fetch_universe(1).has_value());
        ASSERT_FALSE(m_server.fetch_universe(2).has_value());
        ASSERT_TRUE (m_server.fetch_universe(7).has_value());
    }

    {
        // Constant version of fetch functions.
        auto const& server = m_server;

        ASSERT_TRUE (server.fetch_universe(1).has_value());
        ASSERT_FALSE(server.fetch_universe(2).has_value());
        ASSERT_TRUE (server.fetch_universe(7).has_value());
    }

    // Test universes returned by fetch and get are the same.
    ASSERT_EQ(&m_server.get_universe(1), m_server.fetch_universe(1).value());
    ASSERT_EQ(&m_server.get_universe(7), m_server.fetch_universe(7).value());

    {
        size_t universe = 1;
        size_t address  = 218;
        size_t value1   = 64;
        size_t value2   = 128;
        size_t master   = monet::address::to_master_address(universe, address);

        EXPECT_EQ(m_server.get_address_value(universe, address), 0);
        EXPECT_EQ(m_server.get_address_value(master), 0);

        m_server.set_address_value(universe, address, value1);

        EXPECT_EQ(m_server.get_address_value(universe, address), value1);
        EXPECT_EQ(m_server.get_address_value(master), value1);

        m_server.set_address_value(master, value2);

        EXPECT_EQ(m_server.get_address_value(universe, address), value2);
        EXPECT_EQ(m_server.get_address_value(master), value2);
    }

    {
        size_t universe = 7;
        size_t address  = 127;
        size_t value1   = 96;
        size_t value2   = 155;
        size_t master   = monet::address::to_master_address(universe, address);

        EXPECT_EQ(m_server.get_address_value(universe, address), 0);
        EXPECT_EQ(m_server.get_address_value(master), 0);

        m_server.set_address_value(universe, address, value1);

        EXPECT_EQ(m_server.get_address_value(universe, address), value1);
        EXPECT_EQ(m_server.get_address_value(master), value1);

        m_server.set_address_value(master, value2);

        EXPECT_EQ(m_server.get_address_value(universe, address), value2);
        EXPECT_EQ(m_server.get_address_value(master), value2);
    }
}