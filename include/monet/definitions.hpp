//
// Created by maxng on 11/13/2023.
//

#ifndef MASTER_SERVER_DEFINITIONS_HPP
#define MASTER_SERVER_DEFINITIONS_HPP

#include <cstdint>

namespace monet {

    constexpr size_t dmx_data_channel_count = 512;
    constexpr size_t universe_buffer_size = dmx_data_channel_count + 1;

    constexpr uint16_t default_web_panel_port = 8080;

    constexpr size_t default_sink_framerate = 20;

    namespace channel {

        using attribute_channel = size_t;

    }

}

#endif //MASTER_SERVER_DEFINITIONS_HPP
