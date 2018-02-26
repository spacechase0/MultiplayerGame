#include "net/Broadcast/SearchPacket.hpp"

namespace net
{
    namespace Broadcast
    {
        SearchPacket::SearchPacket()
        :   Packet::Packet( Id::Search )
        {
        }

        void SearchPacket::read( sf::Packet& packet )
        {
        }

        void SearchPacket::write( sf::Packet& packet ) const
        {
        }
    }
}
