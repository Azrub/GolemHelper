#pragma once
#include "../Dependencies/mumble/mumble.h"
#include <vector>
#include <string>
#include <map>

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

enum MenuOption {
    // === BOON MENU ===
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
        BOON_RESISTANCE,
        BOON_STABILITY,
        BOON_AEGIS,
        BOON_RETURN2,
    BOON_UTILITY,
        BOON_ALACRITY,
        BOON_QUICKNESS,
        BOON_REGENERATION,
        BOON_SWIFTNESS,
        BOON_VIGOR,
        BOON_EXIT,

    // === HEALER EXTENSIONS ===
    BOON_RETURN3,
    BOON_RETURN4,
    BOON_GOBACK,
    BOON_ADJUSTENVIRONMENT,
    BOON_TOGGLEPULSINGARENADAMAGEON,
    BOON_ENV_MILD,
    BOON_ENV_MODERATE,
    BOON_ENV_EXTREME,

    // === GOLEM MENU ===
    GOLEM_RESPAWNMYPREVIOUSGOLEMINCARNATION,
    GOLEM_SPAWNAGOLEM,
    GOLEM_HITBOX_SMALL,
    GOLEM_HITBOX_MEDIUM,
    GOLEM_HITBOX_LARGE,
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
        GOLEM_IMMOBILIZE,
        GOLEM_GOBACK2,
    GOLEM_COMBATAFFECTINGCONDITIONS,
        GOLEM_BLIND,
        GOLEM_SLOW,
        GOLEM_VULNERABILITY,
        GOLEM_25VULNERABILITY,
        GOLEM_GOBACK3,
        GOLEM_WEAKNESS,
        GOLEM_GOBACK4,
        GOLEM_GOBACK5,
        GOLEM_PLEASESPAWNMYGOLEM,
        GOLEM_EXIT
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
    bool autoShowHideUI = false;
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
    std::map<MenuOption, std::pair<int, int>> coords = {
        // === BOON MENU ===
        {BOON_ADJUSTSELF, {830, 262}},
        {BOON_ADDBOONS, {830, 354}},
        {BOON_OFFENSIVE, {830, 262}},
            {BOON_MIGHT, {830, 262}},
            {BOON_25MIGHT, {830, 400}},
            {BOON_FURY, {830, 305}},
            {BOON_RETURN1, {830, 354}},
        {BOON_DEFENSIVE, {830, 305}},
            {BOON_PROTECTION, {830, 262}},
            {BOON_RESOLUTION, {830, 305}},
            {BOON_RESISTANCE, {830, 354}},
            {BOON_STABILITY, {830, 400}},
            {BOON_AEGIS, {830, 450}},
            {BOON_RETURN2, {830, 500}},
        {BOON_UTILITY, {830, 354}},
            {BOON_ALACRITY, {830, 262}},
            {BOON_QUICKNESS, {830, 305}},
            {BOON_REGENERATION, {830, 354}},
            {BOON_SWIFTNESS, {830, 400}},
            {BOON_VIGOR, {830, 450}},
            {BOON_EXIT, {830, 550}},

            // === HEALER EXTENSIONS ===
            {BOON_RETURN3, {830, 500}},
            {BOON_RETURN4, {830, 450}},
            {BOON_GOBACK, {830, 450}},
            {BOON_ADJUSTENVIRONMENT, {830, 305}},
            {BOON_TOGGLEPULSINGARENADAMAGEON, {830, 262}},
            {BOON_ENV_MILD, {830, 352}},
            {BOON_ENV_MODERATE, {830, 305}},
            {BOON_ENV_EXTREME, {830, 262}},

            // === GOLEM MENU ===
            {GOLEM_RESPAWNMYPREVIOUSGOLEMINCARNATION, {830, 352}},
            {GOLEM_SPAWNAGOLEM, {830, 260}},
            {GOLEM_HITBOX_SMALL, {830, 260}},
            {GOLEM_HITBOX_MEDIUM, {830, 305}},
            {GOLEM_HITBOX_LARGE, {830, 352}},
            {GOLEM_AVERAGEENEMY, {830, 306}},
            {GOLEM_ADDITIONALOPTIONS, {830, 257}},
            {GOLEM_ADDCONDITIONS, {830, 257}},
            {GOLEM_DAMAGEOVERTIMECONDITIONS, {830, 306}},
                {GOLEM_BLEEDING, {830, 257}},
                {GOLEM_BURNING, {830, 306}},
                {GOLEM_CONFUSION, {830, 352}},
                {GOLEM_POISON, {830, 400}},
                {GOLEM_TORMENT, {830, 454}},
                {GOLEM_GOBACK1, {830, 508}},
            {GOLEM_MOBILITYAFFECTINGCONDITIONS, {830, 352}},
                {GOLEM_CHILL, {830, 257}},
                {GOLEM_CRIPPLE, {830, 306}},
                {GOLEM_IMMOBILIZE, {830, 400}},
                {GOLEM_GOBACK2, {830, 454}},
            {GOLEM_COMBATAFFECTINGCONDITIONS, {830, 400}},
                {GOLEM_BLIND, {830, 260}},
                {GOLEM_SLOW, {830, 306}},
                {GOLEM_VULNERABILITY, {830, 352}},
                {GOLEM_25VULNERABILITY, {830, 400}},
                {GOLEM_GOBACK3, {830, 454}},
                {GOLEM_WEAKNESS, {830, 400}},
                {GOLEM_GOBACK4, {830, 454}},
            {GOLEM_GOBACK5, {830, 454}},
            {GOLEM_PLEASESPAWNMYGOLEM, {830, 548}},
            {GOLEM_EXIT, {830, 400}},
    };
};