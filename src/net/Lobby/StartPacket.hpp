#ifndef NET_LOBBY_STARTPACKET_HPP
#define NET_LOBBY_STARTPACKET_HPP

#include "net/Lobby/Packet.hpp"

namespace net
{
    namespace Lobby
    {
        class StartPacket : public Packet
        {
            public:
                StartPacket();

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_LOBBY_STARTPACKET_HPP
