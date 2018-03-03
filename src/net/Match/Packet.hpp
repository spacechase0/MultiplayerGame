#ifndef NET_MATCH_PACKET_HPP
#define NET_MATCH_PACKET_HPP

#include <memory>
#include <SFML/Config.hpp>

#include "net/PacketBase.hpp"

namespace net
{
    namespace Match
    {
        enum Id : sf::Uint8
        {
            GameData = 0,
            CurrentTurn,
            Command,
            EndTurn,
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

#endif // NET_MATCH_PACKET_HPP

