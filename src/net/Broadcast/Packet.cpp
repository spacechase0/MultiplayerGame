#include "net/Broadcast/Packet.hpp"

#include <SFML/Network/Packet.hpp>

#include "net/Broadcast/SearchPacket.hpp"
#include "net/Broadcast/ServerInfoPacket.hpp"

namespace net
{
    namespace Broadcast
    {
        sf::Packet Packet::toPacket() const
        {
            sf::Packet packet;
            packet << static_cast< sf::Uint8 >( id );
            write( packet );
            return packet;
        }

        std::unique_ptr< Packet > Packet::fromPacket( sf::Packet& packet )
        {
            sf::Uint8 id;
            packet >> id;

            std::unique_ptr< Packet > packetObj;
            switch ( static_cast< Id >( id ) )
            {
                case Search: packetObj.reset( new SearchPacket() ); break;
                case ServerInfo: packetObj.reset( new ServerInfoPacket() ); break;
            }

            if ( packetObj )
                packetObj->read( packet );

            return packetObj;
        }
    }
}
