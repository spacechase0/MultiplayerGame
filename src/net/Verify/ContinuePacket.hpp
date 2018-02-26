#ifndef NET_VERIFY_CONTINUEPACKET_HPP
#define NET_VERIFY_CONTINUEPACKET_HPP

#include "net/Verify/Packet.hpp"

namespace net
{
    namespace Verify
    {
        class ContinuePacket : public Packet
        {
            public:
                ContinuePacket();

                virtual void read( sf::Packet& packet ) override;
                virtual void write( sf::Packet& packet ) const override;
        };
    }
}

#endif // NET_VERIFY_CONTINUEPACKET_HPP
