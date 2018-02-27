#include "client/VerifyClientController.hpp"

#include "client/Client.hpp"
#include "Constants.hpp"
#include "net/Verify/Packet.hpp"
#include "net/Verify/VersionPacket.hpp"
#include "net/Verify/ClientUsernamePacket.hpp"

namespace client
{
    void VerifyClientController::update()
    {
        if ( !didFirstUpdate )
        {
            didFirstUpdate = true;
            client.send( net::Verify::VersionPacket().toPacket() );
        }
    }

    void VerifyClientController::onPacket( sf::Packet& packet )
    {
        auto packetObj = net::Verify::Packet::fromPacket( packet );
        if ( packetObj->id == net::Verify::Id::Continue )
        {
            client.send( net::Verify::ClientUsernamePacket( client.username ).toPacket() );
        }
    }
}
