#include "net/Lobby/Packet.hpp"

#include <SFML/Network/Packet.hpp>

#include "net/Lobby/UnitAllocationPacket.hpp"
#include "net/Lobby/StartPacket.hpp"

namespace net
{
    namespace Lobby
    {
        sf::Packet Packet::toPacket() const
        {
            sf::Packet packet;
            packet << static_cast< sf::Uint8 >( id );
            write( packet );
            return packet;
        }

        std::unique_ptr< Packet > Packet::fromPacket( sf::Packet& packet )
        {
            sf::Uint8 id;
            packet >> id;

            std::unique_ptr< Packet > packetObj;
            switch ( static_cast< Id >( id ) )
            {
                case UnitAllocation: packetObj.reset( new UnitAllocationPacket() ); break;
                case Start: packetObj.reset( new StartPacket() ); break;
            }

            if ( packetObj )
                packetObj->read( packet );

            return packetObj;
        }
    }
}
