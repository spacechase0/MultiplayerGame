#ifndef SERVER_CLIENTCONTROLLER_HPP
#define SERVER_CLIENTCONTROLLER_HPP

#include <SFML/Network/Packet.hpp>

namespace server
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

#endif // SERVER_CLIENTCONTROLLER_HPP

