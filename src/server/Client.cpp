#include "server/Client.hpp"

#include "server/ClientController.hpp"
#include "server/VerifyClientController.hpp"

namespace server
{
    Client::Client( std::unique_ptr< sf::TcpSocket > theSocket )
    :   socket( std::move( theSocket ) ),
        controller( new VerifyClientController( * this ) )
    {
    }

    Client::~Client()
    {
    }

    bool Client::isConnected() const
    {
        return socket->getLocalPort() != 0;
    }

    void Client::disconnect()
    {
        socket->disconnect();
    }

    void Client::update()
    {
        if ( controller )
            controller->update();
    }

    void Client::send( sf::Packet packet )
    {
        socket->send( packet );
    }
}