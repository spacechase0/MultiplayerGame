#include "server/Match.hpp"

#include <cmath>
#include <SFML/Network/Packet.hpp>
#include <util/Math.hpp>

#include "Constants.hpp"
#include "game/Unit.hpp"
#include "net/Lobby/StartPacket.hpp"
#include "net/Match/CurrentTurnPacket.hpp"
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
            double sec = 2 * util::PI / clients.size() * i++;
            double baseX = std::cos( sec ) * game::WORLD_SPREAD;
            double baseY = std::sin( sec ) * game::WORLD_SPREAD;

            int j = 0;
            for ( auto& unit : client->units )
            {
                double sec = 2 * util::PI / client->units.size() * j++;
                double armyX = std::cos( sec ) * game::ARMY_SPREAD;
                double armyY = std::sin( sec ) * game::ARMY_SPREAD;

                unit->pos = sf::Vector2d( baseX + armyX, baseY + armyY );
            }

            turns.push( client->id );
        }

        for ( auto& client : clients )
        {
            client->currentMatch = this;
            client->send( net::Lobby::StartPacket( client->id ).toPacket() );
            client->setController( std::unique_ptr< ClientController >( new MatchClientController( server, ( * client ) ) ) );
            client->send( net::Match::CurrentTurnPacket( turns.front() ).toPacket() );
        }
    }

    Match::~Match()
    {
    }

    void Match::update()
    {
        for ( auto& client : clients )
            client->update();
    }

    std::vector< Client* > Match::getClients()
    {
        std::vector< Client* > ret;
        for ( auto& client : clients )
            ret.push_back( client.get() );
        return ret;
    }

    std::vector< const Client* > Match::getClients() const
    {
        std::vector< const Client* > ret;
        for ( auto& client : clients )
            ret.push_back( client.get() );
        return ret;
    }

    sf::Uint64 Match::getCurrentTurn() const
    {
        return turns.front();
    }

    void Match::nextTurn()
    {
        turns.push( turns.front() );
        turns.pop();

        for ( auto& client : clients )
            client->send( net::Match::CurrentTurnPacket( turns.front() ).toPacket() );
    }

    std::vector< game::Unit* > Match::getUnitsAt( sf::Vector2d pos )
    {
        std::vector< game::Unit* > ret;
        for ( auto& client : clients )
            for ( auto& unit : client->units )
                if ( util::distance( unit->pos, pos ) < game::ARMY_UNIT_SIZE )
                    ret.push_back( unit.get() );
        return ret;
    }
}
