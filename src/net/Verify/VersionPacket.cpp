#include "net/Verify/VersionPacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Verify
    {
        VersionPacket::VersionPacket()
        :   Packet::Packet( Id::Version )
        {
        }

        void VersionPacket::read( sf::Packet& packet )
        {
            packet >> version;
        }

        void VersionPacket::write( sf::Packet& packet ) const
        {
            packet << version;
        }
    }
}
