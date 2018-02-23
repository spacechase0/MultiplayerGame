#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <string>
#include <util/String.hpp>

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
        sf::UdpSocket socket;
        socket.setBlocking( true );
        sf::Packet searchPacket = net::Broadcast::SearchPacket().toPacket();
        if ( socket.send( searchPacket, sf::IpAddress::Broadcast, net::BROADCAST_REQ_PORT ) != sf::Socket::Done )
        {
            std::cout << "Error broadcasting search" << std::endl;
            return;
        }

        struct ServerInfo
        {
            sf::IpAddress ip;
            sf::Uint16 port;
            std::string name;
            bool hasPassword;
        };

        bool listening = true;
        std::vector< ServerInfo > found;
        sf::Mutex foundMutex;

        auto listen = [&]()
        {
            sf::UdpSocket listenSocket;
            listenSocket.setBlocking( false );
            if ( listenSocket.bind( net::BROADCAST_RESP_PORT ) != sf::Socket::Done )
            {
                std::cout << "Failed to bind UDP socket" << std::endl;
                return;
            }
            sf::Packet packet;
            while ( listening )
            {
                sf::IpAddress ip;
                sf::Uint16 port;
                if ( listenSocket.receive( packet, ip, port ) == sf::Socket::Done )
                {
                    auto packetObj = net::Broadcast::Packet::fromPacket( packet );
                    if ( packetObj && packetObj->id == net::Broadcast::Id::ServerInfo )
                    {
                        auto info = static_cast< net::Broadcast::ServerInfoPacket* >( packetObj.get() );
                        sf::Lock lock( foundMutex );
                        found.push_back( ServerInfo{ ip, info->port, info->name, info->hasPassword } );
                        std::cout << util::format( "$. $ ([$]:$)" + std::string( info->hasPassword ? " P" : "" ), found.size(), info->name, ip, info->port ) << std::endl;
                    }
                }
                sf::sleep( sf::seconds( 0.1f ) );
            }
        };
        sf::Thread thread( listen );
        thread.launch();
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

