#ifndef SERVER_CLIENT_HPP
#define SERVER_CLIENT_HPP

#include <memory>
#include <SFML/Network/TcpSocket.hpp>

namespace server
{
    class ClientController;
    class Server;

    class Client
    {
        public:
            Client( Server& theServer, std::unique_ptr< sf::TcpSocket > theSocket );
            ~Client();

            bool isConnected() const;
            void disconnect( const std::string& reason = "");

            void update();

            void send( sf::Packet packet );

            const sf::Uint64 id;
            std::string username;

        private:
            Server& server;

            std::unique_ptr< sf::TcpSocket > socket;
            std::unique_ptr< ClientController > controller;

            std::unique_ptr< ClientController > nextController;

            friend class ClientController;
    };
}

#endif // SERVER_CLIENT_HPP
