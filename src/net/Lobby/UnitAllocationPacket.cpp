#include "net/Lobby/UnitAllocationPacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Lobby
    {
        UnitAllocationPacket::UnitAllocationPacket()
        :   Packet::Packet( Id::UnitAllocation )
        {
        }

        UnitAllocationPacket::UnitAllocationPacket( const std::map< game::UnitType, unsigned int >& theAllocation )
        :   Packet::Packet( Id::UnitAllocation ),
            allocation( theAllocation )
        {
        }

        void UnitAllocationPacket::read( sf::Packet& packet )
        {
            allocation.clear();

            sf::Uint32 amt;
            packet >> amt;
            for ( sf::Uint32 i = 0; i < amt; ++i )
            {
                sf::Uint8 type;
                sf::Uint32 count;
                packet >> type >> count;
                allocation[ static_cast< game::UnitType >( type ) ] = count;
            }
        }

        void UnitAllocationPacket::write( sf::Packet& packet ) const
        {
            packet << static_cast< sf::Uint32 >( allocation.size() );
            for ( const auto& alloc : allocation )
                packet << static_cast< sf::Uint8 >( alloc.first ) << alloc.second;
        }
    }
}
