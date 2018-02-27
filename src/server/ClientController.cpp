#include "server/ClientController.hpp"

#include "server/Client.hpp"

namespace server
{
    ClientController::ClientController( Server& theServer, Client& theClient )
    :   server( theServer ),
        client( theClient )
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
