#include "server/ClientController.hpp"

#include "Server/Client.hpp"

namespace server
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
}
