#ifndef NET_MATCH_GAMEDATAPACKET_HPP
#define NET_MATCH_GAMEDATAPACKET_HPP

#include <map>
#include <vector>
#include <string>

#include "Constants.hpp"
#include "game/Unit.hpp"
#include "net/Match/Packet.hpp"

namespace net
{
    namespace Match
    {
        class GameDataPacket : public Packet
        {
            public:
                GameDataPacket();

                struct UnitData
                {
                    game::UnitType type;
                    sf::Vector2d pos;
                    int health;
                };

                std::map< sf::Uint64, std::string > users;
                std::map< sf::Uint64, std::vector< UnitData > > armies;

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_MATCH_GAMEDATAPACKET_HPP
