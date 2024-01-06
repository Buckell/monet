//
// Created by maxng on 1/3/2024.
//

#ifndef MASTER_SERVER_SINK_HPP
#define MASTER_SERVER_SINK_HPP

#include <cstdint>
#include <span>

namespace monet {
    class server;
}

namespace monet::sink {

    /**
     * @brief An interface class for address universe sinks.
     *
     * A sink is a class/construct responsible for taking universe and address
     * data and outputting it externally, such as to a network-based schema like
     * sACN or a physical device like a DMX controller.
     */
    class sink {
        std::string_view m_name;

    protected:
        explicit sink(std::string_view a_name) noexcept :
            m_name(a_name)
        {}

    public:
        sink(sink const&) = delete;
        sink(sink&&)      = delete;

        sink& operator = (sink const&) = delete;
        sink& operator = (sink&&)      = delete;

        virtual ~sink() = default;

        /**
         * @brief Initialize the adapter.
         *
         * @param a_server A reference to the host server instance.
         */
        virtual bool initialize(server& a_server) {
            return true;
        }

        /**
         * @brief deinitialize the adapter.
         */
        virtual void deinitialize() {}

        /**
         * @brief Output data to a specific universe.
         *
         * @param a_universe_number The universe number.
         * @param a_universe        The universe.
         */
        virtual void send_universe(size_t a_universe_number, address::universe const& a_universe) = 0;

        /**
         * @brief Get the sink interface name.
         *
         * @return The sink interface name.
         */
        [[nodiscard]]
        std::string_view name() const noexcept {
            return m_name;
        }
    };

}

#endif //MASTER_SERVER_SINK_HPP
