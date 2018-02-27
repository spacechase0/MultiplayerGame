#include "server/VerifyClientController.hpp"

#include "Constants.hpp"
#include "net/Verify/Packet.hpp"
#include "net/Verify/VersionPacket.hpp"
#include "net/Verify/ContinuePacket.hpp"
#include "net/Verify/ClientUsernamePacket.hpp"
#include "server/Client.hpp"
#include "server/Server.hpp"

namespace server
{
    void VerifyClientController::onPacket( sf::Packet& packet )
    {
        auto packetObj = net::Verify::Packet::fromPacket( packet );
        if ( !verified )
        {
            if ( packetObj->id == net::Verify::Id::Version )
            {
                auto version = static_cast< net::Verify::VersionPacket* >( packetObj.get() );
                if ( version->version != net::PROTOCOL_VERSION )
                {
                    client.disconnect( "bad version" );
                    return;
                }
                verified = true;
                client.send( net::Verify::ContinuePacket().toPacket() );
            }
        }
        else
        {
            if ( packetObj->id == net::Verify::Id::ClientUsername )
            {
                auto username = static_cast< net::Verify::ClientUsernamePacket* >( packetObj.get() );
                client.username = username->username;
                server.log( "Client $ is $\n", client.id, client.username );
            }
        }
    }
}
