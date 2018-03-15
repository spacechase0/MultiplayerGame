#ifndef GAME_UNIT_HPP
#define GAME_UNIT_HPP

#include "Constants.hpp"

namespace game
{
    class IWorldView;

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

            // todo make these private
            sf::Vector2d pos;
            int health;

            void reset( IWorldView* world );

            int getMaxHealth() const;
            void damage( int amt );

            double getMovementSpeedPerTurn() const;
            double getMovementSpeedLeft() const;
            void useMovementSpeed( double amount );
            void moveTo( IWorldView* world, sf::Vector2d worldPos );

            double getAttackRange() const;
            bool hasAttacked() const;
            void attack( IWorldView* world, sf::Vector2d worldPos );

            int getChargeTime() const;
            sf::Vector2d getChargeTarget() const;

        private:
            double movement;
            bool attacked;

            int chargeTime = 0;
            sf::Vector2d chargeTarget;
    };
}

#endif // GAME_UNIT_HPP
