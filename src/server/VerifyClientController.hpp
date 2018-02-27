#ifndef SERVER_VERIFYCLIENTCONTROLLER_HPP
#define SERVER_VERIFYCLIENTCONTROLLER_HPP

#include "server/ClientController.hpp"

namespace server
{
    class VerifyClientController : public ClientController
    {
        public:
            using ClientController::ClientController;

        protected:
            virtual void onPacket( sf::Packet& packet ) override;

        private:
            bool verified = false;
    };
}

#endif // SERVER_VERIFYCLIENTCONTROLLER_HPP
