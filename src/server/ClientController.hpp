#ifndef SERVER_CLIENTCONTROLLER_HPP
#define SERVER_CLIENTCONTROLLER_HPP

#include <memory>
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

            /// Use this instead of setController to avoid this object being destroyed immediately after use (potentially in the middle of a function)
            void controllerTransition( std::unique_ptr< ClientController > newController );

        private:
            sf::Packet pending;
    };
}

#endif // SERVER_CLIENTCONTROLLER_HPP

