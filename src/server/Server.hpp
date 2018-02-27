#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <memory>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <string>
#include <util/Logger.hpp>
#include <vector>

#include "server/LanDiscovery.hpp"

namespace server
{
    class Client;
    class Match;

    class Server
    {
        public:
            Server();
            ~Server();

            void start();

            std::string name = "Game Server";
            std::string password = "";

            util::Logger log;

        private:
            LanDiscovery discovery;

            bool listening = true;
            sf::TcpListener listener;
            sf::Thread listenThread;
            void listen();

            sf::Mutex clientsMutex;
            std::vector< std::unique_ptr< Client > > clients;

            std::vector< std::unique_ptr< Match > > matches;

            sf::Thread mainThread;
            void main();
    };
}

#endif // SERVER_SERVER_HPP
