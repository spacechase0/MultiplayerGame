#include "net/Verify/ClientUsernamePacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Verify
    {
        ClientUsernamePacket::ClientUsernamePacket()
        :   Packet::Packet( Id::ClientUsername )
        {
        }

        void ClientUsernamePacket::read( sf::Packet& packet )
        {
            packet >> username;
        }

        void ClientUsernamePacket::write( sf::Packet& packet ) const
        {
            packet << username;
        }
    }
}
