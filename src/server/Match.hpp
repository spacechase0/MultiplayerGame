#ifndef SERVER_MATCH_HPP
#define SERVER_MATCH_HPP

#include <memory>
#include <vector>
#include <queue>
#include <SFML/Config.hpp>

#include "game/IWorldView.hpp"

namespace server
{
    class Client;
    class Server;

    class Match : public game::IWorldView
    {
        public:
            Match( Server& theServer, std::vector< std::unique_ptr< Client > > theClients );
            ~Match();

            void update();

            std::vector< Client* > getClients();
            std::vector< const Client* > getClients() const;

            sf::Uint64 getCurrentTurn() const;
            void nextTurn();

            virtual std::vector< game::Unit* > getUnitsAt( sf::Vector2d pos ) override;

        private:
            Server& server;
            std::vector< std::unique_ptr< Client > > clients;

            std::queue< sf::Uint64 > turns;

            friend class Server;
    };
}

#endif // SERVER_MATCH_HPP
