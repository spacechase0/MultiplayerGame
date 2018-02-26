#ifndef NET_VERIFY_PACKET_HPP
#define NET_VERIFY_PACKET_HPP

#include <memory>
#include <SFML/Config.hpp>

#include "net/PacketBase.hpp"

namespace net
{
    namespace Verify
    {
        enum Id : sf::Uint8
        {
            Version = 0,
            Continue,
            ClientUsername,
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

#endif // NET_VERIFY_PACKET_HPP

