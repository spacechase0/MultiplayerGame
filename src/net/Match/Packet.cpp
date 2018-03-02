#include "net/Match/Packet.hpp"

#include <SFML/Network/Packet.hpp>

#include "net/Match/GameDataPacket.hpp"
#include "net/Match/CurrentTurnPacket.hpp"

namespace net
{
    namespace Match
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
                case GameData: packetObj.reset( new GameDataPacket() ); break;
                case CurrentTurn: packetObj.reset( new CurrentTurnPacket() ); break;
            }

            if ( packetObj )
                packetObj->read( packet );

            return packetObj;
        }
    }
}
