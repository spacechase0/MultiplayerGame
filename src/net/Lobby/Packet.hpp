#ifndef NET_LOBBY_PACKET_HPP
#define NET_LOBBY_PACKET_HPP

#include <memory>
#include <SFML/Config.hpp>

#include "net/PacketBase.hpp"

namespace net
{
    namespace Lobby
    {
        enum Id : sf::Uint8
        {
            UnitAllocation = 0,
            Start,
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

#endif // NET_LOBBY_PACKET_HPP

