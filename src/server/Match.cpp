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

        for ( auto& client : clients )
        {
            if ( client->id == turns.front() )
            {
                for ( auto& unit : client->units )
                {
                    unit->reset( this );
                }
            }
        }
    }

    std::vector< game::Unit* > Match::getUnitsAt( sf::Vector2d pos )
    {
        return getUnitsWithin( pos, game::ARMY_UNIT_SIZE );
    }

    std::vector< game::Unit* > Match::getUnitsWithin( sf::Vector2d pos, double dist )
    {
        std::vector< game::Unit* > ret;
        for ( auto& client : clients )
            for ( auto& unit : client->units )
                if ( util::distance( unit->pos, pos ) < dist )
                    ret.push_back( unit.get() );
        return ret;
    }

    std::vector< game::Unit* > Match::getUnitsIntersecting( sf::Vector2d start, sf::Vector2d end )
    {
        sf::Vector2d left = start, right = end;
        sf::Vector2d top = start, bottom = end;
        if ( right.x < left.x ) std::swap( left, right );
        if ( bottom.y < top.y ) std::swap( top, bottom );

        std::vector< game::Unit* > ret;
        for ( auto&client : clients )
            for ( auto& unit : client->units )
            {
                // https://stackoverflow.com/a/23017208/1687492
                double dx = end.x - start.x;
                double dy = end.y - start.y;

                double a = dx * dx + dy * dy;
                double b = 2 * ( dx * ( start.x - unit->pos.x ) + dy * ( start.y - unit->pos.y ) );
                double c = ( start.x - unit->pos.x ) * ( start.x - unit->pos.x ) + ( start.y - unit->pos.y ) * ( start.y - unit->pos.y ) - game::ARMY_UNIT_SIZE * game::ARMY_UNIT_SIZE;

                double det = b * b - 4 * a * c;
                if ( a <= 0.0000001 || det < 0 )
                {
                    continue;
                }
                else
                {
                    if ( unit->pos.x < left.x || unit->pos.x > right.x ||
                         unit->pos.y < top.y || unit->pos.y > bottom.y )
                        continue;
                    ret.push_back( unit.get() );
                }
            }
        return ret;
    }
}
