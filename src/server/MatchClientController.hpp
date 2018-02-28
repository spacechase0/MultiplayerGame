#ifndef SERVER_MATCHCLIENTCONTROLLER_HPP
#define SERVER_MATCHCLIENTCONTROLLER_HPP

#include "server/ClientController.hpp"

namespace server
{
    class MatchClientController : public ClientController
    {
        public:
            MatchClientController( Server& theServer, Client& theClient );

            virtual void update() override;

            void sync();

        protected:
            virtual void onPacket( sf::Packet& packet ) override;
    };
}

#endif // SERVER_MATCHCLIENTCONTROLLER_HPP
