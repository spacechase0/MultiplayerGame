#ifndef SERVER_LANDISCOVERY_HPP
#define SERVER_LANDISCOVERY_HPP

#include <functional>
#include <SFML/Config.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Thread.hpp>
#include <string>

namespace server
{
    class Server;

    class LanDiscovery
    {
        public:
            LanDiscovery( Server& theServer );
            ~LanDiscovery();

            void start();
            void stop();

        private:
            Server& server;

            bool running = false;
            sf::Thread thread;

            void impl();
    };
}

#endif // SERVER_LANDISCOVERY_HPP
