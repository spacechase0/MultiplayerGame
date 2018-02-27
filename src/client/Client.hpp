#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <memory>
#include <SFML/Network/TcpSocket.hpp>
#include <string>

namespace client
{
    class ClientController;

    class Client
    {
        public:
            Client( std::unique_ptr< sf::TcpSocket > theSocket, std::string theUsername );
            ~Client();

            bool isConnected() const;

            void send( sf::Packet packet );

            void run();
            void stop();

            const std::string username;

        private:
            std::unique_ptr< sf::TcpSocket > socket;
            std::unique_ptr< ClientController > controller;
            bool running;

            friend class ClientController;
    };
}

#endif // CLIENT_CLIENT_HPP
