#include "server/MatchClientController.hpp"

#include "net/Match/GameDataPacket.hpp"
#include "server/Client.hpp"
#include "server/Match.hpp"
#include "server/Server.hpp"

namespace server
{
    MatchClientController::MatchClientController( Server& theServer, Client& theClient )
    :   ClientController::ClientController( theServer, theClient )
    {
        sync();
    }

    void MatchClientController::update()
    {
        ClientController::update();
    }

    void MatchClientController::sync()
    {
        net::Match::GameDataPacket packet;
        for ( const auto& client : client.currentMatch->getClients() )
        {
            packet.users[ client->id ] = client->username;
            for ( const auto& unit : client->units )
            {
                net::Match::GameDataPacket::UnitData data;
                data.type = unit->type;
                data.pos = unit->pos;
                data.health = unit->health;
                packet.armies[ client->id ].push_back( data );
            }
        }

        client.send( packet.toPacket() );
    }

    void MatchClientController::onPacket( sf::Packet& packet )
    {
    }
}
