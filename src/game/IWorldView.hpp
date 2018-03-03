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
    };
}

#endif // GAME_IWORLDVIEW_HPP
