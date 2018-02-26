#ifndef NET_BROADCAST_SERVERINFOPACKET_HPP
#define NET_BROADCAST_SERVERINFOPACKET_HPP

#include <SFML/Config.hpp>
#include <string>

#include "Constants.hpp"
#include "net/Broadcast/Packet.hpp"

namespace net
{
    namespace Broadcast
    {
        class ServerInfoPacket : public Packet
        {
            public:
                ServerInfoPacket();

                sf::Uint16 port = net::GAME_PORT;
                std::string name = "Server";
                bool hasPassword = false;

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_BROADCAST_SERVERINFOPACKET_HPP
