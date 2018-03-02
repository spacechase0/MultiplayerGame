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
            int health;

            double getMovementSpeedLeft() const;
            void useMovementSpeed( double amount );
            void moveTo( sf::Vector2d worldPos );

        private:
            double movement = 5;
    };
}

#endif // GAME_UNIT_HPP
