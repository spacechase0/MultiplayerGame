#ifndef NET_MATCH_CURRENTTURNPACKET_HPP
#define NET_MATCH_CURRENTTURNPACKET_HPP

#include <map>
#include <vector>
#include <string>

#include "Constants.hpp"
#include "game/Unit.hpp"
#include "net/Match/Packet.hpp"

namespace net
{
    namespace Match
    {
        class CurrentTurnPacket : public Packet
        {
            public:
                CurrentTurnPacket();
                CurrentTurnPacket( sf::Uint64 theCurrent );

                sf::Uint64 current;

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_MATCH_CURRENTTURNPACKET_HPP
