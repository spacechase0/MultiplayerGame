#ifndef NET_BROADCAST_PACKET_HPP
#define NET_BROADCAST_PACKET_HPP

#include <memory>
#include <SFML/Config.hpp>

#include "net/PacketBase.hpp"

namespace net
{
    namespace Broadcast
    {
        enum Id : sf::Uint8
        {
            Search = 0,
            ServerInfo = 1,
        };

        class Packet : public PacketBase< Id >
        {
            public:
                sf::Packet toPacket() const;
                static std::unique_ptr< Packet > fromPacket( sf::Packet& packet );

            protected:
                using PacketBase::PacketBase;
        };
    }
}

#endif // NET_BROADCAST_PACKET_HPP

