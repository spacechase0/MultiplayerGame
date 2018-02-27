#ifndef CLIENT_VERIFYCLIENTCONTROLLER_HPP
#define CLIENT_VERIFYCLIENTCONTROLLER_HPP

#include "client/ClientController.hpp"

namespace client
{
    class VerifyClientController : public ClientController
    {
        public:
            using ClientController::ClientController;

            virtual void update() override;

        protected:
            virtual void onPacket( sf::Packet& packet );

        private:
            bool didFirstUpdate = false;
            bool sentUsername = false;
    };
}

#endif // CLIENT_VERIFYCLIENTCONTROLLER_HPP
