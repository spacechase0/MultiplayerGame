#ifndef CLIENT_LOBBYCLIENTCONTROLLER_HPP
#define CLIENT_LOBBYCLIENTCONTROLLER_HPP

#include "client/ClientController.hpp"

namespace client
{
    class LobbyClientController : public ClientController
    {
        public:
            using ClientController::ClientController;

        protected:
            virtual void onPacket( sf::Packet& packet );
    };
}

#endif // CLIENT_LOBBYCLIENTCONTROLLER_HPP
