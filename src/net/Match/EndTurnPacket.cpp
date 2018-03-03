#include "net/Match/EndTurnPacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace net
{
    namespace Match
    {
        EndTurnPacket::EndTurnPacket()
        :   Packet::Packet( Id::EndTurn )
        {
        }

        void EndTurnPacket::read( sf::Packet& packet )
        {
        }

        void EndTurnPacket::write( sf::Packet& packet ) const
        {
        }
    }
}
