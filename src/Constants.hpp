#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Config.hpp>

namespace net
{
    constexpr sf::Uint16 PROTOCOL_VERSION = 0;
    constexpr sf::Uint16 BROADCAST_REQ_PORT = 12346;
    constexpr sf::Uint16 BROADCAST_RESP_PORT = 12347;
    constexpr sf::Uint16 GAME_PORT = 12345;
}

namespace game
{
    constexpr int UNIT_POINTS = 10;
    constexpr int WORLD_SPREAD = 500;
    constexpr int ARMY_SPREAD = 50;
}

#endif // CONSTANTS_HPP
