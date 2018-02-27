#ifndef SERVER_LOBBYCLIENTCONTROLLER_HPP
#define SERVER_LOBBYCLIENTCONTROLLER_HPP

#include "server/ClientController.hpp"

namespace server
{
    class LobbyClientController : public ClientController
    {
        public:
            using ClientController::ClientController;

        protected:
            virtual void onPacket( sf::Packet& packet );
    };
}

#endif // SERVER_LOBBYCLIENTCONTROLLER_HPP
