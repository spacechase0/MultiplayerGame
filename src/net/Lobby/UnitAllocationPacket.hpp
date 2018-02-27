#ifndef NET_LOBBY_UNITALLOCATIONPACKET_HPP
#define NET_LOBBY_UNITALLOCATIONPACKET_HPP

#include <map>

#include "Constants.hpp"
#include "game/Unit.hpp"
#include "net/Lobby/Packet.hpp"

namespace net
{
    namespace Lobby
    {
        class UnitAllocationPacket : public Packet
        {
            public:
                UnitAllocationPacket();
                UnitAllocationPacket( const std::map< game::UnitType, sf::Uint32 >& theAllocation );

                std::map< game::UnitType, sf::Uint32 > allocation;

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_LOBBY_UNITALLOCATIONPACKET_HPP
