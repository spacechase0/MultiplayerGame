#include "net/Verify/ContinuePacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Verify
    {
        ContinuePacket::ContinuePacket()
        :   Packet::Packet( Id::Continue )
        {
        }

        void ContinuePacket::read( sf::Packet& packet )
        {
        }

        void ContinuePacket::write( sf::Packet& packet ) const
        {
        }
    }
}
