#include "game/Unit.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <util/Math.hpp>
#include <util/String.hpp>

#include "game/IWorldView.hpp"

namespace game
{
    Unit::Unit( UnitType theType )
    :   type( theType )
    {
        health = getMaxHealth();
        attacked = false;
        movement = getMovementSpeedPerTurn();
    }

    void Unit::reset( IWorldView* world )
    {
        if ( health <= 0 )
            return;
        if ( chargeTime > 0 )
        {
            --chargeTime;
            if ( chargeTime == 0 )
            {
                if ( type == UnitType::Mage )
                {
                    auto unitsFar = world->getUnitsWithin( chargeTarget, 4 );
                    for ( auto& unit : unitsFar )
                        unit->damage( 3 );

                    auto unitsClose = world->getUnitsWithin( chargeTarget, 2 );
                    for ( auto& unit : unitsClose )
                        unit->damage( 5 );
                }
            }
            movement = 0;
        }

        attacked = false;
        movement = getMovementSpeedPerTurn();
    }

    int Unit::getMaxHealth() const
    {
        switch ( type )
        {
            case UnitType::Scout: return 8;
            case UnitType::Fighter: return 15;
            case UnitType::Mage: return 4;
            case UnitType::Archer: return 6;
        }

        throw std::runtime_error( util::format( "This should never happen. $:$: $\n", __FILE__, __LINE__, __func__ ) );
    }

    void Unit::damage( int amt )
    {
        health = std::max( health - amt, 0 );
    }

    double Unit::getMovementSpeedPerTurn() const
    {
        switch ( type )
        {
            case UnitType::Scout: return 10;
            case UnitType::Fighter: return 7;
            case UnitType::Mage: return 5;
            case UnitType::Archer: return 5;
        }

        throw std::runtime_error( util::format( "This should never happen. $:$: $\n", __FILE__, __LINE__, __func__ ) );
    }

    double Unit::getMovementSpeedLeft() const
    {
        return movement;
    }

    void Unit::useMovementSpeed( double amount )
    {
        movement -= amount;
    }

    void Unit::moveTo( IWorldView* world, sf::Vector2d worldPos )
    {
        if ( health <= 0 )
            return;
        if ( chargeTime > 0 )
            return;

        double angle = std::atan2( worldPos.y - pos.y, worldPos.x - pos.x );
        double dist = std::min( util::distance( pos, worldPos ), getMovementSpeedLeft() );
        pos.x += std::cos( angle ) * dist;
        pos.y += std::sin( angle ) * dist;
        useMovementSpeed( dist );
    }

    double Unit::getAttackRange() const
    {
        switch ( type )
        {
            case UnitType::Scout: return 2;
            case UnitType::Fighter: return 3;
            case UnitType::Mage: return 15;
            case UnitType::Archer: return 25;
        }

        throw std::runtime_error( util::format( "This should never happen. $:$: $\n", __FILE__, __LINE__, __func__ ) );
    }

    bool Unit::hasAttacked() const
    {
        return attacked;
    }

    void Unit::attack( IWorldView* world, sf::Vector2d worldPos )
    {
        if ( health <= 0 )
            return;
        if ( chargeTime > 0 )
            return;
        if ( attacked )
            return;
        attacked = true;

        double angle = std::atan2( worldPos.y - pos.y, worldPos.x - pos.x );
        double dist = std::min( util::distance( pos, worldPos ), getAttackRange() );
        sf::Vector2d spot = pos;
        spot.x += std::cos( angle ) * dist;
        spot.y += std::sin( angle ) * dist;

        switch ( type )
        {
            case UnitType::Fighter:
            case UnitType::Scout:
                {
                    auto units = world->getUnitsAt( spot );
                    for ( auto& unit : units )
                        unit->damage( type == UnitType::Fighter ? 4 : 2 );
                }
                break;

            case UnitType::Mage:
                chargeTime = 2;
                chargeTarget = spot;
                movement = 0;
                break;

            case UnitType::Archer:
                // todo
                break;
        }

        if ( movement != getMovementSpeedPerTurn() )
            movement = 0;
    }

    int Unit::getChargeTime() const
    {
        return chargeTime;
    }

    sf::Vector2d Unit::getChargeTarget() const
    {
        return chargeTarget;
    }
}
