#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <memory>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <string>
#include <vector>

#include "server/Client.hpp"
#include "server/LanDiscovery.hpp"

namespace server
{
    class Server
    {
        public:
            Server();

            void start();

            std::string name = "Game Server";

        private:
            LanDiscovery discovery;

            sf::TcpListener listener;
            sf::Thread listenThread;
            void listen();

            sf::Mutex clientsMutex;
            std::vector< std::unique_ptr< Client > > clients;

            sf::Thread mainThread;
            void main();
    };
}

#endif // SERVER_SERVER_HPP
