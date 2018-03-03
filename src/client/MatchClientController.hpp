#ifndef CLIENT_MATCHCLIENTCONTROLLER_HPP
#define CLIENT_MATCHCLIENTCONTROLLER_HPP

#include <map>
#include <memory>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <string>

#include "client/ClientController.hpp"
#include "game/IWorldView.hpp"
#include "game/Unit.hpp"

namespace client
{
    class MatchClientController : public ClientController, public game::IWorldView
    {
        public:
            MatchClientController( Client& theClient );

            virtual void update() override;

            virtual std::vector< game::Unit* > getUnitsAt( sf::Vector2d pos ) override;

        protected:
            virtual void onPacket( sf::Packet& packet ) override;

        private:
            sf::RenderWindow window;
            sf::View view;

            sf::Font font;

            std::map< sf::Uint64, std::string > users;
            std::map< sf::Uint64, std::vector< std::unique_ptr< game::Unit > > > armies;

            enum MouseMode
            {
                Select,
                Move,
                Attack,
            } mouseMode = Select;

            game::Unit* selected = nullptr;

            sf::Uint64 currentTurn;
    };
}

#endif // CLIENT_MATCHCLIENTCONTROLLER_HPP
