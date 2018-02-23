#ifndef CLIENT_LANDISCOVERY_HPP
#define CLIENT_LANDISCOVERY_HPP

#include <functional>
#include <SFML/Config.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Thread.hpp>
#include <string>

namespace client
{
    class LanDiscovery
    {
        public:
            LanDiscovery();
            ~LanDiscovery();

            struct ServerInfo
            {
                sf::IpAddress ip;
                sf::Uint16 port;
                std::string name;
                bool hasPassword;
            };

            bool start();
            void stop();

            std::function< void ( ServerInfo ) > onDiscovery;

        private:
            bool running = false;
            sf::Thread thread;

            void impl();
    };
}

#endif // CLIENT_LANDISCOVERY_HPP
