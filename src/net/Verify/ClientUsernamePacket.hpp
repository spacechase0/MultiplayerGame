#ifndef NET_VERIFY_CLIENTUSERNAMEPACKET_HPP
#define NET_VERIFY_CLIENTUSERNAMEPACKET_HPP

#include "Constants.hpp"
#include "net/Verify/Packet.hpp"

namespace net
{
    namespace Verify
    {
        class ClientUsernamePacket : public Packet
        {
            public:
                ClientUsernamePacket();
                ClientUsernamePacket( const std::string& theUsername );

                std::string username;

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_VERIFY_CLIENTUSERNAMEPACKET_HPP
