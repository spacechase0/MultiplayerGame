#ifndef SERVER_MATCH_HPP
#define SERVER_MATCH_HPP

#include <memory>
#include <vector>

namespace server
{
    class Client;
    class Server;

    class Match
    {
        public:
            Match( Server& theServer, std::vector< std::unique_ptr< Client > > theClients );
            ~Match();

            void update();

            std::vector< Client* > getClients();
            std::vector< const Client* > getClients() const;

        private:
            Server& server;
            std::vector< std::unique_ptr< Client > > clients;

            friend class Server;
    };
}

#endif // SERVER_MATCH_HPP
