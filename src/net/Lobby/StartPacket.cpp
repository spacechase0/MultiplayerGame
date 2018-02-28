#include "net/Lobby/StartPacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Lobby
    {
        StartPacket::StartPacket()
        :   Packet::Packet( Id::Start )
        {
        }

        StartPacket::StartPacket( sf::Uint64 theId )
        :   Packet::Packet( Id::Start ),
            id( theId )
        {
        }

        void StartPacket::read( sf::Packet& packet )
        {
            packet >> id;
        }

        void StartPacket::write( sf::Packet& packet ) const
        {
            packet << id;
        }
    }
}
