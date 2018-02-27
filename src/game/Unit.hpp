#ifndef GAME_UNIT_HPP
#define GAME_UNIT_HPP

#include "Constants.hpp"

namespace game
{
    enum UnitType
    {
        Scout = 0,
        Fighter,
        Mage,
        Archer,
    };

    class Unit
    {
        public:
            Unit( UnitType theType );

            const UnitType type;

            sf::Vector2d pos;
    };
}

#endif // GAME_UNIT_HPP
