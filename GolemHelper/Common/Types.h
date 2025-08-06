#pragma once
#include "../Dependencies/mumble/mumble.h"
#include <vector>
#include <string>

enum HitboxType {
    HITBOX_SMALL = 0,
    HITBOX_MEDIUM = 1,
    HITBOX_LARGE = 2
};

enum EnvironmentDamageLevel {
    ENV_MILD = 0,
    ENV_MODERATE = 1,
    ENV_EXTREME = 2
};

struct GolemTemplate {
    std::string name;
    bool isQuickDps;
    bool isAlacDps;
    bool environmentDamage;
    EnvironmentDamageLevel envDamageLevel;
    bool skipBurning;
    bool skipConfusion;
    bool skipSlow;
    bool addImmobilize;
    bool addBlind;
    bool fiveBleedingStacks;
    HitboxType hitboxType;
    bool isDefaultTemplate;
    bool addResistance;
    bool addStability;
    bool skipAegis;

    GolemTemplate() :
        name("Unnamed Template"),
        isQuickDps(false),
        isAlacDps(false),
        environmentDamage(false),
        envDamageLevel(ENV_MILD),
        skipBurning(false),
        skipConfusion(false),
        skipSlow(false),
        addImmobilize(false),
        addBlind(false),
        fiveBleedingStacks(false),
        hitboxType(HITBOX_SMALL),
        isDefaultTemplate(false),
        addResistance(false),
        addStability(false),
        skipAegis(false) {
    }
};

struct GolemHelperState {
    bool enabled = false;
    bool isQuickDps = false;
    bool isAlacDps = false;
    bool environmentDamage = false;
    EnvironmentDamageLevel envDamageLevel = ENV_MILD;
    bool skipBurning = false;
    bool skipConfusion = false;
    bool skipSlow = false;
    bool addImmobilize = false;
    bool addBlind = false;
    bool fiveBleedingStacks = false;
    HitboxType hitboxType = HITBOX_SMALL;
    bool debugMode = false;
    bool showUI = false;
    bool showAdvanced = false;
    bool showTimingSettings = false;
    bool showBoonAdvanced = false;
    bool addResistance = false;
    bool addStability = false;
    bool skipAegis = false;
    bool alwaysHideIcon = false;
    int debugCounter = 0;

    int initialDelay = 390;
    int stepDelay = 290;

    bool quickAccessVisible = false;
    unsigned int lastMapID = 0;

    std::vector<GolemTemplate> templates;
    int selectedTemplateIndex = -1;
    int lastUserTemplateIndex = -1;
    char newTemplateName[64] = "";
};

struct MenuCoordinates {
    int golemStepX[24] = {
        830, 830, 830, 830, 830, 830, 830, 830, 830, 830,
        830, 830, 830, 830, 830, 830, 830, 830, 830, 830,
        830, 830, 830, 830
    };

    int golemStepY[24] = {
        260, 260, 306, 257, 257, 306, 257, 306, 352, 400,
        454, 508, 352, 257, 306, 454, 400, 306, 352, 400,
        454, 454, 454, 548
    };

    int boonStepX[20] = {
        830, 830, 830, 830, 830, 830, 830, 830, 830, 830,
        830, 830, 830, 830, 830, 830, 830, 830, 830, 830
    };

    int boonStepY[20] = {
        262, 354, 262, 262, 400, 305, 354, 305, 262, 305,
        450, 500, 354, 262, 305, 354, 400, 450, 262, 550
    };

    int healerStepX[10] = {
        830, 830, 830, 830, 830, 830, 830, 830, 830, 830
    };

    int healerStepY[10] = {
        262, 352, 352, 262, 262, 500, 450, 450, 305, 262
    };

    int immobilizeX = 830;
    int immobilizeY = 400;
    int blindX = 830;
    int blindY = 260;

    int resistanceX = 830;
    int resistanceY = 354;
    int stabilityX = 830;
    int stabilityY = 400;
};