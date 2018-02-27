#include "server/Client.hpp"

#include <util/Logger.hpp>

#include "server/ClientController.hpp"
#include "server/VerifyClientController.hpp"
#include "server/Server.hpp"

namespace server
{
    sf::Uint64 clientIdCounter = 0;

    Client::Client( Server& theServer, std::unique_ptr< sf::TcpSocket > theSocket )
    :   id( clientIdCounter++ ),
        server( theServer ),
        socket( std::move( theSocket ) ),
        controller( new VerifyClientController( theServer, ( * this ) ) )
    {
        server.log( "Client $ connected.\n", id );
    }

    Client::~Client()
    {
    }

    bool Client::isConnected() const
    {
        return socket->getLocalPort() != 0;
    }

    void Client::disconnect( const std::string& reason )
    {
        server.log( "Disconnecting client $ for $\n", id, reason );
        socket->disconnect();
    }

    void Client::update()
    {
        if ( nextController )
            controller = std::move( nextController );

        if ( controller )
            controller->update();
    }

    void Client::send( sf::Packet packet )
    {
        socket->send( packet );
    }
}
