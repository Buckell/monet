//
// Created by maxng on 12/13/2023.
//

#include <ranges>

#include <monet.hpp>

namespace monet::channel::attribute {

    attribute_channel attribute::channel_name_to_id(std::string_view const a_channel_name) const noexcept {
        auto channels = available_channels();
        auto it = std::find(channels.begin(), channels.end(), a_channel_name);

        // Return ID offset or 0 if name not found.
        return (it != channels.end()) ? it - channels.begin() : 0;
    }

    std::string_view attribute::channel_id_to_name(attribute_channel const a_channel_id) const noexcept {
        auto channels = available_channels();

        // Return "base" if ID out of range.
        if (a_channel_id >= channels.size()) [[unlikely]] {
            return "base";
        }

        return channels[a_channel_id];
    }

    bool attribute::channel_name_exists(std::string_view a_channel_name) const noexcept {
        auto channels = available_channels();
        return std::find(channels.begin(), channels.end(), a_channel_name) != channels.end();
    }

    bool attribute::channel_id_exists(monet::channel::attribute_channel a_channel_id) const noexcept {
        return a_channel_id < available_channels().size();
    }
}