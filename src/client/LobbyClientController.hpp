#ifndef CLIENT_LOBBYCLIENTCONTROLLER_HPP
#define CLIENT_LOBBYCLIENTCONTROLLER_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "client/ClientController.hpp"
#include "game/Unit.hpp"

namespace client
{
    class Client;

    class LobbyClientController : public ClientController
    {
        public:
            LobbyClientController( Client& theClient );

            virtual void update();

        protected:
            virtual void onPacket( sf::Packet& packet );

            bool allocating = true;

            sf::RenderWindow window;
            sf::Font font;

            int points = UNIT_POINTS;
            std::map< game::UnitType, int > allocated;
    };
}

#endif // CLIENT_LOBBYCLIENTCONTROLLER_HPP
