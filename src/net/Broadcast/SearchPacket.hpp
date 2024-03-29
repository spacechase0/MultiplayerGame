#ifndef NET_BROADCAST_SEARCHPACKET_HPP
#define NET_BROADCAST_SEARCHPACKET_HPP

#include "net/Broadcast/Packet.hpp"

namespace net
{
    namespace Broadcast
    {
        class SearchPacket : public Packet
        {
            public:
                SearchPacket();

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_BROADCAST_SEARCHPACKET_HPP
