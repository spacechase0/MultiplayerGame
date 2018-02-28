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
                StartPacket( sf::Uint64 theId );

                sf::Uint64 id; ///< The ID of the client that this is being sent to.

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_LOBBY_STARTPACKET_HPP
