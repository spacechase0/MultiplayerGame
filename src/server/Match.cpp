#include "server/Match.hpp"

#include <SFML/Network/Packet.hpp>

#include "net/Lobby/StartPacket.hpp"
#include "server/Client.hpp"
#include "server/MatchClientController.hpp"

namespace server
{
    Match::Match( Server& theServer, std::vector< std::unique_ptr< Client > > theClients )
    :   server( theServer ),
        clients( std::move( theClients ) )
    {
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
