#ifndef SERVER_CLIENT_HPP
#define SERVER_CLIENT_HPP

#include <memory>
#include <SFML/Network/TcpSocket.hpp>

namespace server
{
    class ClientController;

    class Client
    {
        public:
            Client( std::unique_ptr< sf::TcpSocket > theSocket );
            ~Client();

            bool isConnected() const;
            void disconnect();

            void update();

            void send( sf::Packet packet );

            std::string username;

        private:
            std::unique_ptr< sf::TcpSocket > socket;
            std::unique_ptr< ClientController > controller;

            friend class ClientController;
    };
}

#endif // SERVER_CLIENT_HPP
