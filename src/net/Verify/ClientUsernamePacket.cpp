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

        ClientUsernamePacket::ClientUsernamePacket( const std::string& theUsername )
        :   Packet::Packet( Id::ClientUsername ),
            username( theUsername )
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
