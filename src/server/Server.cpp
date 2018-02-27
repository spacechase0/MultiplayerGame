#include "server/Server.hpp"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Sleep.hpp>

#include "Constants.hpp"
#include "server/Client.hpp"
#include "server/Match.hpp"

namespace server
{
    Server::Server()
    :   log( "server.log" ),
        discovery( * this ),
        listenThread( &Server::listen, this ),
        mainThread( &Server::main, this )
    {
    }

    Server::~Server()
    {
    }

    void Server::start()
    {
        discovery.start();
        listenThread.launch();
        mainThread.launch();
    }

    void Server::listen()
    {
        listener.listen( net::GAME_PORT );

        while ( listening )
        {
            auto client = std::make_unique< sf::TcpSocket >();
            if ( listener.accept( * client ) == sf::Socket::Done )
            {
                client->setBlocking( false );
                sf::Lock lock( clientsMutex );
                clients.push_back( std::make_unique< Client >( ( * this ), std::move( client ) ) );
            }
        }
    }

    void Server::main()
    {
        while ( true )
        {
            {
                sf::Lock lock( clientsMutex );
                bool ready = true;
                for ( auto& client : clients )
                {
                    client->update();
                    if ( client->units.size() == 0 )
                        ready = false;
                }

                if ( ready && clients.size() >= 2 )
                {
                    listening = false;
                    listener.close();
                    discovery.stop();

                    log( "Starting match" );
                    auto match = std::make_unique< Match >( ( * this ), std::move( clients ) );
                    matches.push_back( std::move( match ) );
                }
            }

            for ( auto& match : matches )
                match->update();

            sf::sleep( sf::seconds( 1.f / 100 ) );
        }
    }
}
