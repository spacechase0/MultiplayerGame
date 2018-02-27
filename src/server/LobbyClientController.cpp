#include "server/LobbyClientController.hpp"

#include "net/Lobby/Packet.hpp"
#include "net/Lobby/UnitAllocationPacket.hpp"
#include "server/Client.hpp"
#include "server/Server.hpp"

namespace server
{
    void LobbyClientController::onPacket( sf::Packet& packet )
    {
        auto packetObj = net::Lobby::Packet::fromPacket( packet );
        if ( packetObj->id == net::Lobby::Id::UnitAllocation )
        {
            auto allocation = static_cast< net::Lobby::UnitAllocationPacket* >( packetObj.get() );
            if ( client.units.size() == 0 )
            {
                server.log( "$ allocated units\n", client.username );
                for ( const auto& alloc : allocation->allocation )
                    for ( unsigned int i = 0; i < alloc.second; ++i )
                        client.units.push_back( std::make_unique< game::Unit >( alloc.first ) );
            }
        }
    }
}
