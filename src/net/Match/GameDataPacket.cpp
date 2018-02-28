#include "net/Match/GameDataPacket.hpp"

#include <SFML/Network/Packet.hpp>

namespace
{
    sf::Packet& operator << ( sf::Packet& packet, const net::Match::GameDataPacket::UnitData& unit )
    {
        packet << static_cast< sf::Uint8 >( unit.type ) << unit.pos.x << unit.pos.y << unit.health;
        return packet;
    }

    sf::Packet& operator >> ( sf::Packet& packet, net::Match::GameDataPacket::UnitData& unit )
    {
        sf::Uint8 unitType;
        packet >> unitType >> unit.pos.x >> unit.pos.y >> unit.health;
        unit.type = static_cast< game::UnitType >( unitType );
        return packet;
    }
}

namespace net
{
    namespace Match
    {
        GameDataPacket::GameDataPacket()
        :   Packet::Packet( Id::GameData )
        {
        }

        void GameDataPacket::read( sf::Packet& packet )
        {
            users.clear();
            armies.clear();

            sf::Uint32 amt;
            packet >> amt;
            for ( sf::Uint32 i = 0; i < amt; ++i )
            {
                sf::Uint64 id;
                std::string username;
                packet >> id >> username;
                users[ id ] = username;
            }

            packet >> amt;
            for ( sf::Uint32 i = 0; i < amt; ++i )
            {
                sf::Uint64 id;
                sf::Uint32 armySize;
                packet >> id >> armySize;
                for ( sf::Uint32 i = 0; i < armySize; ++i )
                {
                    UnitData unit;
                    packet >> unit;
                    armies[ id ].push_back( unit );
                }
            }
        }

        void GameDataPacket::write( sf::Packet& packet ) const
        {
            packet << static_cast< sf::Uint32 >( users.size() );
            for ( const auto& user : users )
                packet << user.first << user.second;

            packet << static_cast< sf::Uint32 >( armies.size() );
            for ( const auto& army : armies )
            {
                packet << army.first << static_cast< sf::Uint32 >( army.second.size() );
                for ( const auto& unit : army.second )
                    packet << unit;
            }
        }
    }
}
