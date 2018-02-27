#include "client/ClientController.hpp"

#include "client/Client.hpp"

namespace client
{
    ClientController::ClientController( Client& theClient )
    :   client( theClient )
    {
    }

    ClientController::~ClientController()
    {
    }

    void ClientController::update()
    {
        if ( client.socket->receive( pending ) == sf::Socket::Done )
        {
            onPacket( pending );
            pending = sf::Packet();
        }
    }

    void ClientController::controllerTransition( std::unique_ptr< ClientController > newController )
    {
        client.nextController = std::move( newController );
    }
}
