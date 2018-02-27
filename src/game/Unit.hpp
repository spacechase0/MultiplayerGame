#ifndef GAME_UNIT_HPP
#define GAME_UNIT_HPP

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
    };
}

#endif // GAME_UNIT_HPP
