#include "net/Broadcast/ServerInfoPacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Broadcast
    {
        ServerInfoPacket::ServerInfoPacket()
        :   Packet::Packet( Id::ServerInfo )
        {
        }

        void ServerInfoPacket::read ( sf::Packet& packet )
        {
            packet >> port >> name >> hasPassword;
        }

        void ServerInfoPacket::write ( sf::Packet& packet ) const
        {
            packet << port << name << hasPassword;
        }
    }
}
