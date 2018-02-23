#ifndef SERVER_LANDISCOVERY_HPP
#define SERVER_LANDISCOVERY_HPP

#include <functional>
#include <SFML/Config.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Thread.hpp>
#include <string>

namespace server
{
    class LanDiscovery
    {
        public:
            LanDiscovery();
            ~LanDiscovery();

            std::string name = "Server";
            bool hasPassword = false;

            bool start();
            void stop();

        private:
            bool running = false;
            sf::Thread thread;

            void impl();
    };
}

#endif // SERVER_LANDISCOVERY_HPP
