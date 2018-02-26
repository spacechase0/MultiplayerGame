#include "net/Verify/Packet.hpp"

#include <SFML/Network/Packet.hpp>

#include "net/Verify/VersionPacket.hpp"
#include "net/Verify/ContinuePacket.hpp"
#include "net/Verify/ClientUsernamePacket.hpp"

namespace net
{
    namespace Verify
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
                case Version: packetObj.reset( new VersionPacket() ); break;
                case Continue: packetObj.reset( new ContinuePacket() ); break;
                case ClientUsername: packetObj.reset( new ClientUsernamePacket() ); break;
            }

            if ( packetObj )
                packetObj->read( packet );

            return packetObj;
        }
    }
}
