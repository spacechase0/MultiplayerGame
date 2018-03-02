#include "net/Match/CurrentTurnPacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Match
    {
        CurrentTurnPacket::CurrentTurnPacket()
        :   Packet::Packet( Id::CurrentTurn )
        {
        }

        CurrentTurnPacket::CurrentTurnPacket( sf::Uint64 theCurrent )
        :   Packet::Packet( Id::CurrentTurn ),
            current( theCurrent )
        {
        }

        void CurrentTurnPacket::read( sf::Packet& packet )
        {
            packet >> current;
        }

        void CurrentTurnPacket::write( sf::Packet& packet ) const
        {
            packet << current;
        }
    }
}
