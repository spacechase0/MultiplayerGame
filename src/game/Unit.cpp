#include "game/Unit.hpp"

#include <algorithm>
#include <cmath>
#include <util/Math.hpp>

namespace game
{
    Unit::Unit( UnitType theType )
    :   type( theType )
    {
    }

    double Unit::getMovementSpeedLeft() const
    {
        return movement;
    }

    void Unit::useMovementSpeed( double amount )
    {
        movement -= amount;
    }

    void Unit::moveTo( sf::Vector2d worldPos )
    {
        double angle = std::atan2( worldPos.y - pos.y, worldPos.x - pos.x );
        double dist = std::min( util::distance( pos, worldPos ), getMovementSpeedLeft() );
        pos.x += std::cos( angle ) * dist;
        pos.y += std::sin( angle ) * dist;
        useMovementSpeed( dist );
    }
}
