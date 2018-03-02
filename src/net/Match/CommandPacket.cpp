#include "net/Match/CommandPacket.hpp"

#include <SFML/Network/Packet.hpp>

#include"iostream"
namespace net
{
    namespace Match
    {
        CommandPacket::CommandPacket()
        :   Packet::Packet( Id::Command )
        {
        }

        void CommandPacket::read( sf::Packet& packet )
        {
            sf::Uint8 cmdType;
            packet >> cmdType >> withUnit >> pos.x >> pos.y;
            type = static_cast< Type >( cmdType );
        }

        void CommandPacket::write( sf::Packet& packet ) const
        {
            packet << static_cast< sf::Uint8 >( type ) << withUnit << pos.x << pos.y;
        }
    }
}
