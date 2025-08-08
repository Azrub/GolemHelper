#pragma once
#include "Types.h"

class MenuSequences {
public:
    static constexpr MenuOption BOON_SEQUENCE[20] = {
        BOON_ADJUSTSELF,
        BOON_ADDBOONS,
        BOON_OFFENSIVE,
        BOON_MIGHT,
        BOON_25MIGHT,
        BOON_FURY,
        BOON_RETURN1,
        BOON_DEFENSIVE,
        BOON_PROTECTION,
        BOON_RESOLUTION,
        BOON_AEGIS,
        BOON_RETURN2,
        BOON_UTILITY,
        BOON_ALACRITY,
        BOON_QUICKNESS,
        BOON_REGENERATION,
        BOON_SWIFTNESS,
        BOON_VIGOR,
        BOON_ALACRITY,
        BOON_EXIT
    };

    static constexpr MenuOption HEALER_QUICK_SEQUENCE[10] = {
        BOON_ADJUSTSELF,
        BOON_ADDBOONS,
        BOON_UTILITY,
        BOON_ALACRITY,
        BOON_ALACRITY,
        BOON_RETURN3,
        BOON_RETURN4,
        BOON_GOBACK,
        BOON_ADJUSTENVIRONMENT,
        BOON_TOGGLEPULSINGARENADAMAGEON
    };

    static constexpr MenuOption GOLEM_SEQUENCE[25] = {
        GOLEM_SPAWNAGOLEM,
        GOLEM_HITBOX_SMALL,
        GOLEM_AVERAGEENEMY,
        GOLEM_ADDITIONALOPTIONS,
        GOLEM_ADDCONDITIONS,
        GOLEM_DAMAGEOVERTIMECONDITIONS,
        GOLEM_BLEEDING,
        GOLEM_BURNING,
        GOLEM_CONFUSION,
        GOLEM_POISON,
        GOLEM_TORMENT,
        GOLEM_GOBACK1,
        GOLEM_MOBILITYAFFECTINGCONDITIONS,
        GOLEM_CHILL,
        GOLEM_CRIPPLE,
        GOLEM_GOBACK2,
        GOLEM_COMBATAFFECTINGCONDITIONS,
        GOLEM_SLOW,
        GOLEM_VULNERABILITY,
        GOLEM_25VULNERABILITY,
        GOLEM_GOBACK3,
        GOLEM_WEAKNESS,
        GOLEM_GOBACK4,
        GOLEM_GOBACK5,
        GOLEM_PLEASESPAWNMYGOLEM
    };

    static constexpr int BOON_LENGTH = 20;
    static constexpr int HEALER_QUICK_LENGTH = 10;
    static constexpr int GOLEM_LENGTH = 25;
};