#include "server/LanDiscovery.hpp"

#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Sleep.hpp>

#include "Constants.hpp"
#include "net/Broadcast/SearchPacket.hpp"
#include "net/Broadcast/ServerInfoPacket.hpp"
#include "server/Server.hpp"

namespace server
{
    LanDiscovery::LanDiscovery( Server& theServer )
    :   server( theServer ),
        thread( &LanDiscovery::impl, this )
    {
    }

    LanDiscovery::~LanDiscovery()
    {
        stop();
    }

    void LanDiscovery::start()
    {
        sf::UdpSocket socket;
        socket.setBlocking( true );
        sf::Packet searchPacket = net::Broadcast::SearchPacket().toPacket();
        socket.send( searchPacket, sf::IpAddress::Broadcast, net::BROADCAST_REQ_PORT );

        if ( !running )
        {
            running = true;
            thread.launch();
        }
    }

    void LanDiscovery::stop()
    {
        running = false;
    }

    void LanDiscovery::impl()
    {
        sf::UdpSocket listenSocket;
        listenSocket.setBlocking( false );
        if ( listenSocket.bind( net::BROADCAST_REQ_PORT ) != sf::Socket::Done )
        {
            running = false;
            return;
        }

        sf::Packet packet;
        while ( running )
        {
            sf::IpAddress ip;
            sf::Uint16 port;
            if ( listenSocket.receive( packet, ip, port ) == sf::Socket::Done )
            {
                auto packetObj = net::Broadcast::Packet::fromPacket( packet );
                if ( packetObj && packetObj->id == net::Broadcast::Id::Search )
                {
                    net::Broadcast::ServerInfoPacket info;
                    info.name = server.name;
                    info.port = net::GAME_PORT;
                    info.hasPassword = server.password != "";

                    sf::Packet send = info.toPacket();

                    sf::UdpSocket socket;
                    if ( socket.send( send, ip, net::BROADCAST_RESP_PORT ) != sf::Socket::Done )
                    {
                    }
                }
            }
            sf::sleep( sf::seconds( 0.1f ) );
        }
    }
}
