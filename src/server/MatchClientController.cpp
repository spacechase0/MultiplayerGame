#include "server/MatchClientController.hpp"

#include "net/Match/CommandPacket.hpp"
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
        auto packetObj = net::Match::Packet::fromPacket( packet );
        if ( packetObj->id == net::Match::Command )
        {
            auto cmd = static_cast< net::Match::CommandPacket* >( packetObj.get() );
            if ( client.currentMatch->getCurrentTurn() != client.id )
                return;

            switch ( cmd->type )
            {
                case net::Match::CommandPacket::Move:
                    server.log( "$ moved unit $ to ($, $)\n", client.username, static_cast< int >( cmd->withUnit ), cmd->pos.x, cmd->pos.y );
                    client.units[ cmd->withUnit ]->moveTo( client.currentMatch, cmd->pos );
                    break;

                case net::Match::CommandPacket::Attack:
                    server.log( "$ attacked with unit $ at ($, $)\n", client.username, static_cast< int >( cmd->withUnit ), cmd->pos.x, cmd->pos.y );
                    client.units[ cmd->withUnit ]->attack( client.currentMatch, cmd->pos );
                    break;
            }

            for ( auto& otherClient : client.currentMatch->getClients() )
                if ( otherClient != &client )
                    otherClient->send( cmd->toPacket() );
        }
    }
}
