#include "client/MatchClientController.hpp"

#include "client/Client.hpp"
#include "Constants.hpp"

namespace client
{
    void MatchClientController::update()
    {
        ClientController::update();
    }

    void MatchClientController::onPacket( sf::Packet& packet )
    {
    }
}
