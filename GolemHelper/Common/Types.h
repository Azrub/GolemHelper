#pragma once

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

struct GolemHelperState {
    bool enabled = false;
    bool boonsEnabled = true;
    bool golemEnabled = true;
    bool isQuickDps = false;
    bool isAlacDps = false;
    bool environmentDamage = false;
    EnvironmentDamageLevel envDamageLevel = ENV_MILD;
    bool skipSlow = false;
    bool skipBurning = false;
    bool fiveBleedingStacks = false;
    HitboxType hitboxType = HITBOX_SMALL;
    bool debugMode = false;
    bool showUI = false;
    bool useCustomDelays = false;
    bool showAdvanced = false;
    bool showTimingSettings = false;
    int debugCounter = 0;

    int initialDelay = 390;
    int stepDelay = 290;    
};

struct MenuCoordinates {
    int golemStepX[25] = {
        830, 830, 830, 830, 830, 830, 830, 830, 830, 830,
        830, 830, 830, 830, 830, 830, 830, 830, 830, 830,
        830, 830, 830, 830, 830
    };
    int golemStepY[25] = {
        260, 260, 306, 257, 257, 306, 257, 306, 352, 400,
        454, 508, 352, 257, 306, 454, 400, 306, 352, 400,
        454, 400, 454, 454, 548
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
};