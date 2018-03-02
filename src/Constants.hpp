#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    typedef sf::Vector2< double > Vector2d;
}

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

    constexpr double WORLD_UNIT_SIZE = 16;
    constexpr int WORLD_SPREAD = 30;
    constexpr int ARMY_SPREAD = 3;
}

#endif // CONSTANTS_HPP
