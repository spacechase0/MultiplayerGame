#include "net/Broadcast/SearchPacket.hpp"

namespace net
{
    namespace Broadcast
    {
        SearchPacket::SearchPacket()
        :   Packet::Packet( Id::Search )
        {
        }

        SearchPacket::read ( sf::Packet& packet )
        {
        }

        SearchPacket::write ( sf::Packet& packet ) const
        {
        }
    }
}
