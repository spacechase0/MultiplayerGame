#include "server/Server.hpp"

#include <SFML/System/Lock.hpp>
#include <SFML/System/Sleep.hpp>

#include "Constants.hpp"

namespace server
{
    Server::Server()
    :   listenThread( &Server::listen, this ),
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
            sf::Lock lock( clientsMutex );
            clients.push_back( std::move( client ) );
        }
    }

    void Server::main()
    {
        while ( true )
            sf::sleep( sf::seconds( 0.1f ) );
    }
}
