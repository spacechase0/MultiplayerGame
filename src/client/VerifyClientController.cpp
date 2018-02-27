#include "client/VerifyClientController.hpp"

#include "client/Client.hpp"
#include "client/LobbyClientController.hpp"
#include "Constants.hpp"
#include "net/Verify/Packet.hpp"
#include "net/Verify/VersionPacket.hpp"
#include "net/Verify/ClientUsernamePacket.hpp"

namespace client
{
    void VerifyClientController::update()
    {
        ClientController::update();

        if ( !didFirstUpdate )
        {
            didFirstUpdate = true;
            client.log( "Sending version verify packet...\n" );
            client.send( net::Verify::VersionPacket().toPacket() );
        }
    }

    void VerifyClientController::onPacket( sf::Packet& packet )
    {
        auto packetObj = net::Verify::Packet::fromPacket( packet );
        if ( packetObj->id == net::Verify::Id::Continue )
        {
            if ( !sentUsername )
            {
                client.log( "Sending username...\n" );
                client.send( net::Verify::ClientUsernamePacket( client.username ).toPacket() );
                sentUsername = true;
            }
            else
            {
                client.log( "Moving to lobby state\n" );
                controllerTransition( std::unique_ptr< ClientController >( new LobbyClientController( client ) ) );
            }
        }
    }
}
