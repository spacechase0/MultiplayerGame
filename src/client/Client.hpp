#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <memory>
#include <SFML/Network/TcpSocket.hpp>
#include <string>

namespace client
{
    class Client
    {
        public:
            Client( std::unique_ptr< sf::TcpSocket > theSocket, std::string theUsername );

            bool isConnected() const;

            const std::string username;

        private:
            std::unique_ptr< sf::TcpSocket > socket;
    };
}

#endif // CLIENT_CLIENT_HPP
