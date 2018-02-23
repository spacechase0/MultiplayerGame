#ifndef NET_PACKET_HPP
#define NET_PACKET_HPP

namespace sf
{
    class Packet;
}

namespace net
{
    template< typename PACKET_ID >
    class PacketBase
    {
        public:
            virtual ~PacketBase() {}

            const PACKET_ID id;


        protected:
            PacketBase( PACKET_ID theId ) : id( theId ) {};

            virtual read ( sf::Packet& packet ) = 0;
            virtual write ( sf::Packet& packet ) const = 0;
    };
}

#endif // NET_PACKET_HPP
