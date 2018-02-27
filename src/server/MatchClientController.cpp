#include "server/MatchClientController.hpp"

#include "server/Client.hpp"
#include "server/Server.hpp"

namespace server
{
    void MatchClientController::update()
    {
        ClientController::update();
    }

    void MatchClientController::onPacket( sf::Packet& packet )
    {
    }
}
