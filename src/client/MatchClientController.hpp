#ifndef CLIENT_MATCHCLIENTCONTROLLER_HPP
#define CLIENT_MATCHCLIENTCONTROLLER_HPP

#include <map>
#include <memory>
#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <string>

#include "client/ClientController.hpp"
#include "game/Unit.hpp"

namespace client
{
    class MatchClientController : public ClientController
    {
        public:
            MatchClientController( Client& theClient );

            virtual void update() override;

        protected:
            virtual void onPacket( sf::Packet& packet ) override;

        private:
            sf::RenderWindow window;
            sf::View view;

            std::map< sf::Uint64, std::string > users;
            std::map< sf::Uint64, std::vector< std::unique_ptr< game::Unit > > > armies;

            game::Unit* selected = nullptr;

            sf::Uint64 currentTurn;
    };
}

#endif // CLIENT_MATCHCLIENTCONTROLLER_HPP
