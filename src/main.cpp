#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <string>
#include <util/String.hpp>

#include "client/LanDiscovery.hpp"
#include "Constants.hpp"
#include "net/Broadcast/Packet.hpp"
#include "net/Broadcast/SearchPacket.hpp"
#include "net/Broadcast/ServerInfoPacket.hpp"

void runClient()
{
    std::string ip;
    std::cout << "Server IP: ";
    std::getline( std::cin, ip );

    if ( ip == "" )
    {
        client::LanDiscovery lanDisco;
        std::vector< client::LanDiscovery::ServerInfo > found;
        sf::Mutex foundMutex;

        lanDisco.onDiscovery = [&]( client::LanDiscovery::ServerInfo info )
        {
            found.push_back( info );
            std::cout << util::format( "$. $ ([$]:$)" + std::string( info.hasPassword ? " P" : "" ), found.size(), info.name, ip, info.port ) << std::endl;
        };
        lanDisco.start();

        while ( true )
            sf::sleep( sf::seconds( 0.1f ) );
    }
}

void runServer()
{
    bool searchListening = true;
    auto listenSearch = [&]()
    {
        sf::UdpSocket listenSocket;
        listenSocket.setBlocking( false );
        if ( listenSocket.bind( net::BROADCAST_REQ_PORT ) != sf::Socket::Done )
        {
            std::cout << "Failed to bind UDP socket" << std::endl;
            return;
        }

        sf::Packet packet;
        while ( searchListening )
        {
            sf::IpAddress ip;
            sf::Uint16 port;
            if ( listenSocket.receive( packet, ip, port ) == sf::Socket::Done )
            {
                auto packetObj = net::Broadcast::Packet::fromPacket( packet );
                if ( packetObj && packetObj->id == net::Broadcast::Id::Search )
                {
                    net::Broadcast::ServerInfoPacket info;
                    info.name = "Server";
                    info.port = net::GAME_PORT;
                    info.hasPassword = false;

                    sf::Packet send = info.toPacket();

                    sf::UdpSocket socket;
                    if ( socket.send( send, ip, net::BROADCAST_RESP_PORT ) != sf::Socket::Done )
                    {
                        std::cout << "Failed to send response to server search" << std::endl;
                    }
                }
            }
            sf::sleep( sf::seconds( 0.1f ) );
        }
    };
    sf::Thread searchThread( listenSearch );
    searchThread.launch();
}

int main()
{
    std::string line;

    std::cout << "Server or client: ";
    std::getline( std::cin, line );
    if ( line[0] == 'c' )
    {
        runClient();
    }
    else if ( line[0] == 's' )
    {
        runServer();
    }
}

