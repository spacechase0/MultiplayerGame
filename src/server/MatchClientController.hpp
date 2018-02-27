#ifndef SERVER_MATCHCLIENTCONTROLLER_HPP
#define SERVER_MATCHCLIENTCONTROLLER_HPP

#include "server/ClientController.hpp"

namespace server
{
    class MatchClientController : public ClientController
    {
        public:
            using ClientController::ClientController;

            virtual void update() override;

        protected:
            virtual void onPacket( sf::Packet& packet ) override;
    };
}

#endif // SERVER_MATCHCLIENTCONTROLLER_HPP
