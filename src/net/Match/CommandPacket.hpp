#ifndef NET_MATCH_COMMANDPACKET_HPP
#define NET_MATCH_COMMANDPACKET_HPP

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
        class CommandPacket : public Packet
        {
            public:
                CommandPacket();

                enum Type
                {
                    Move,
                    Attack,
                } type;

                sf::Uint8 withUnit;
                sf::Vector2d pos;

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_MATCH_COMMANDPACKET_HPP
