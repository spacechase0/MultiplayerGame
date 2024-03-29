#ifndef GAME_IWORLDVIEW_HPP
#define GAME_IWORLDVIEW_HPP

#include <vector>

#include "Constants.hpp"

namespace game
{
    class Unit;

    class IWorldView
    {
        public:
            virtual ~IWorldView();

            virtual std::vector< Unit* > getUnitsAt( sf::Vector2d spot ) = 0;
            virtual std::vector< Unit* > getUnitsWithin( sf::Vector2d spot, double dist ) = 0;
            virtual std::vector< Unit* > getUnitsIntersecting( sf::Vector2d start, sf::Vector2d end ) = 0;
    };
}

#endif // GAME_IWORLDVIEW_HPP
