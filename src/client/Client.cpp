#include "client/Client.hpp"

namespace client
{
    Client::Client( std::unique_ptr< sf::TcpSocket > theSocket, std::string theName )
    :   name( theName ),
        socket( std::move( theSocket ) )
    {
    }

    bool Client::isConnected() const
    {
        return socket->getLocalPort() != 0;
    }
}
