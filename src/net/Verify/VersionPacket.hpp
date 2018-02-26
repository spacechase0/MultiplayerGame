#ifndef NET_VERIFY_VERSIONPACKET_HPP
#define NET_VERIFY_VERSIONPACKET_HPP

#include <SFML/Config.hpp>

#include "Constants.hpp"
#include "net/Verify/Packet.hpp"

namespace net
{
    namespace Verify
    {
        class VersionPacket : public Packet
        {
            public:
                VersionPacket();

                sf::Uint16 version = PROTOCOL_VERSION;

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_VERIFY_VERSIONPACKET_HPP
