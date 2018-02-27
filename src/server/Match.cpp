#include "server/Match.hpp"

#include <cmath>
#include <SFML/Network/Packet.hpp>
#include <util/Math.hpp>

#include "Constants.hpp"
#include "game/Unit.hpp"
#include "net/Lobby/StartPacket.hpp"
#include "server/Client.hpp"
#include "server/MatchClientController.hpp"

namespace server
{
    Match::Match( Server& theServer, std::vector< std::unique_ptr< Client > > theClients )
    :   server( theServer ),
        clients( std::move( theClients ) )
    {
        int i = 0;
        for ( auto& client : clients )
        {
            int sec = 2 * util::PI / clients.size() * i++;
            double baseX = std::cos( sec ) * game::WORLD_SPREAD;
            double baseY = std::sin( sec ) * game::WORLD_SPREAD;

            int j = 0;
            for ( auto& unit : client->units )
            {
                int sec = 2 * util::PI / client->units.size() * j++;
                double armyX = std::cos( sec ) * game::ARMY_SPREAD;
                double armyY = std::sin( sec ) * game::ARMY_SPREAD;

                unit->pos = sf::Vector2d( baseX + armyX, baseY + armyY );
            }
        }

        for ( auto& client : clients )
        {
            client->send( net::Lobby::StartPacket().toPacket() );
            client->setController( std::unique_ptr< ClientController >( new MatchClientController( server, ( * client ) ) ) );
        }
    }

    Match::~Match()
    {
    }

    void Match::update()
    {
    }
}
