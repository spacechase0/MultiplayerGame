#ifndef CLIENT_MATCHCLIENTCONTROLLER_HPP
#define CLIENT_MATCHCLIENTCONTROLLER_HPP

#include "client/ClientController.hpp"

namespace client
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

#endif // CLIENT_MATCHCLIENTCONTROLLER_HPP
