#ifndef CLIENT_CLIENTCONTROLLER_HPP
#define CLIENT_CLIENTCONTROLLER_HPP

#include <memory>
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

            void controllerTransition( std::unique_ptr< ClientController > newController );

        private:
            sf::Packet pending;
    };
}

#endif // CLIENT_CLIENTCONTROLLER_HPP

