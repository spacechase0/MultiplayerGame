#include "server/Server.hpp"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Sleep.hpp>

#include "Constants.hpp"

namespace server
{
    Server::Server()
    :   log( "server.log" ),
        discovery( * this ),
        listenThread( &Server::listen, this ),
        mainThread( &Server::main, this )
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

        auto client = std::make_unique< sf::TcpSocket >();
        if ( listener.accept( * client ) == sf::Socket::Done )
        {
            client->setBlocking( false );
            sf::Lock lock( clientsMutex );
            clients.push_back( std::make_unique< Client >( ( * this ), std::move( client ) ) );
        }
    }

    void Server::main()
    {

        while ( true )
        {
            for ( auto& client : clients )
                client->update();

            sf::sleep( sf::seconds( 1.f / 100 ) );
        }
    }
}
