#include <SFML/Network.hpp>
#include <string>
#include <iostream>

void runClient()
{
}

void runServer()
{
}

int main()
{
    std::string line;

    std::cout << "Server or client: ";
    std::getline( std::cin, line );
    if ( line[0] == 'c' )
    {
        runClient();
    }
    else if ( line[0] == 's' )
    {
        runServer();
    }
}

