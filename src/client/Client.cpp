#include "client/Client.hpp"

#include <SFML/System/Sleep.hpp>

#include "client/VerifyClientController.hpp"

namespace client
{
    Client::Client( std::unique_ptr< sf::TcpSocket > theSocket, std::string theUsername )
    :   username( theUsername ),
        socket( std::move( theSocket ) ),
        controller( new VerifyClientController( * this ) )
    {
    }

    Client::~Client()
    {
    }

    bool Client::isConnected() const
    {
        return socket->getLocalPort() != 0;
    }

    void Client::send( sf::Packet packet )
    {
        socket->send( packet );
    }

    void Client::run()
    {
        running = true;
        while ( running )
        {
            controller->update();
            sf::sleep( sf::seconds( 1.f / 100 ) );
        }
    }

    void Client::stop()
    {
        running = false;
    }
}
