#ifndef SERVER_CLIENTCONTROLLER_HPP
#define SERVER_CLIENTCONTROLLER_HPP

#include <SFML/Network/Packet.hpp>

namespace server
{
    class Client;
    class Server;

    class ClientController
    {
        public:
            ClientController( Server& theServer, Client& theClient );
            virtual ~ClientController();

            virtual void update();

        protected:
            Server& server;
            Client& client;

            virtual void onPacket( sf::Packet& packet ) = 0;

        private:
            sf::Packet pending;
    };
}

#endif // SERVER_CLIENTCONTROLLER_HPP

