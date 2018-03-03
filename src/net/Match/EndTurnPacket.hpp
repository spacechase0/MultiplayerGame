#ifndef NET_MATCH_ENDTURNPACKET_HPP
#define NET_MATCH_ENDTURNPACKET_HPP

#include "net/Match/Packet.hpp"

namespace net
{
    namespace Match
    {
        class EndTurnPacket : public Packet
        {
            public:
                EndTurnPacket();

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_MATCH_ENDTURNPACKET_HPP
