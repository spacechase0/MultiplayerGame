#include "server/Server.hpp"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Sleep.hpp>

#include "Constants.hpp"

namespace server
{
    Server::Server()
    :   discovery( * this ),
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
            sf::Lock lock( clientsMutex );
            clients.push_back( std::make_unique< Client >( std::move( client ) ) );
        }
    }

    void Server::main()
    {
        for ( auto& client : clients )
            client->update();

        while ( true )
            sf::sleep( sf::seconds( 1.f / 100 ) );
    }
}
