#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <string>
#include <util/String.hpp>

#include "client/Client.hpp"
#include "client/LanDiscovery.hpp"
#include "Constants.hpp"
#include "net/Broadcast/Packet.hpp"
#include "net/Broadcast/SearchPacket.hpp"
#include "net/Broadcast/ServerInfoPacket.hpp"
#include "server/LanDiscovery.hpp"
#include "server/Server.hpp"

void runClient()
{
    std::string ipStr;
    std::cout << "Server IP: ";
    std::getline( std::cin, ipStr );

    sf::IpAddress ip;
    int port;
    if ( ipStr == "" )
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

        unsigned int choice;
        std::cin >> choice;

        ip = found[ choice - 1 ].ip;
        port = found[ choice - 1 ].port;
    }
    else
    {
        ip = sf::IpAddress( ipStr );

        std::cout << "Server Port: ";
        std::cin >> port;
    }

    std::string username;
    std::cout << "Username: ";
    std::cin >> username;

    auto socket = std::make_unique< sf::TcpSocket >();
    if ( socket->connect( ip, port ) != sf::Socket::Done )
    {
        std::cout << "Failed to connect!" << std::endl;
        return;
    }
    std::cout << "Connected." << std::endl;

    client::Client client( std::move( socket ), username );

    while ( true )
        sf::sleep( sf::seconds( 0.1f ) );
}

void runServer()
{
    server::Server server;
    server.start();

    while ( true )
        sf::sleep( sf::seconds( 0.1f ) );
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

