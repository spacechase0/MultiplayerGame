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

        void StartPacket::read( sf::Packet& packet )
        {
        }

        void StartPacket::write( sf::Packet& packet ) const
        {
        }
    }
}
