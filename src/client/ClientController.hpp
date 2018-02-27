#ifndef CLIENT_CLIENTCONTROLLER_HPP
#define CLIENT_CLIENTCONTROLLER_HPP

#include <SFML/Network/Packet.hpp>

namespace client
{
    class Client;

    class ClientController
    {
        public:
            ClientController( Client& theClient );
            virtual ~ClientController();

            virtual void update();

        protected:
            Client& client;

            virtual void onPacket( sf::Packet& packet ) = 0;

        private:
            sf::Packet pending;
    };
}

#endif // CLIENT_CLIENTCONTROLLER_HPP

