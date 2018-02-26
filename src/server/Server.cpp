#include "server/Server.hpp"

#include <SFML/System/Lock.hpp>

#include "Constants.hpp"

namespace server
{
    Server::Server()
    :   listenThread( &Server::listen, this )
    {
    }

    void Server::start()
    {
        discovery.start();
        listenThread.launch();
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
}
