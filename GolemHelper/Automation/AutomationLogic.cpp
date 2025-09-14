#include <Windows.h>
#include <string>
#include "AutomationLogic.h"
#include "../Common/Globals.h"
#include "../Common/MenuSequences.h"
#include "CoordinateUtils.h"

bool AutomationLogic::ShouldSkipBoonStep(int stepIndex) {
    MenuOption step = MenuSequences::BOON_SEQUENCE[stepIndex];

    if (g_state.isQuickDps && step == BOON_QUICKNESS) {
        return true;
    }

    if (g_state.isAlacDps && step == BOON_ALACRITY) {
        return true;
    }

    if (g_state.showBoonAdvanced && g_state.skipAegis && step == BOON_AEGIS) {
        return true;
    }

    return false;
}

bool AutomationLogic::ShouldSkipGolemStep(int stepIndex) {
    if (!g_state.showAdvanced) {
        return false;
    }

    MenuOption step = MenuSequences::GOLEM_SEQUENCE[stepIndex];

    if (g_state.skipBurning && step == GOLEM_BURNING) {
        return true;
    }

    if (g_state.skipConfusion && step == GOLEM_CONFUSION) {
        return true;
    }

    if (g_state.skipSlow && step == GOLEM_SLOW) {
        return true;
    }

    return false;
}

void AutomationLogic::ApplyHealerBoons() {
    if (!g_api || !g_state.enabled) return;

    bool uiWasVisible = g_state.showUI;
    if (uiWasVisible) {
        g_state.showUI = false;
    }

    std::string mode = "Healer Bench - ";
    if (g_state.isQuickDps) {
        mode += "Quick DPS";
    }
    else if (g_state.isAlacDps) {
        mode += "Alac DPS";
    }

    mode += " + Environment ";
    switch (g_state.envDamageLevel) {
    case ENV_MILD: mode += "Mild"; break;
    case ENV_MODERATE: mode += "Moderate"; break;
    case ENV_EXTREME: mode += "Extreme"; break;
    }

    char startBuffer[400];
    sprintf_s(startBuffer, "Starting healer boon sequence - Mode: %s", mode.c_str());
    g_api->Log(ELogLevel_INFO, "GolemHelper", startBuffer);

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(g_state.initialDelay);

        bool alacrityCounted = false;
        for (int i = 0; i < MenuSequences::HEALER_QUICK_LENGTH; i++) {
            MenuOption step = MenuSequences::HEALER_QUICK_SEQUENCE[i];

            if (g_state.isAlacDps && step == BOON_ALACRITY) {
                if (!alacrityCounted) {
                    auto quicknessCoord = g_coords.coords.find(BOON_QUICKNESS);
                    if (quicknessCoord != g_coords.coords.end()) {
                        CoordinateUtils::ClickAtScaled(
                            quicknessCoord->second.first,
                            quicknessCoord->second.second,
                            g_state.stepDelay
                        );
                    }
                    alacrityCounted = true;
                }
                continue;
            }

            auto coordIt = g_coords.coords.find(step);
            if (coordIt != g_coords.coords.end()) {
                CoordinateUtils::ClickAtScaled(
                    coordIt->second.first,
                    coordIt->second.second,
                    g_state.stepDelay
                );
            }
        }

        MenuOption envDamageOption;
        switch (g_state.envDamageLevel) {
        case ENV_MILD: envDamageOption = BOON_ENV_MILD; break;
        case ENV_MODERATE: envDamageOption = BOON_ENV_MODERATE; break;
        case ENV_EXTREME: envDamageOption = BOON_ENV_EXTREME; break;
        default: envDamageOption = BOON_ENV_MILD; break;
        }

        auto envCoord = g_coords.coords.find(envDamageOption);
        if (envCoord != g_coords.coords.end()) {
            CoordinateUtils::ClickAtScaled(envCoord->second.first, envCoord->second.second, 50);
        }
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during healer boon sequence");
    }

    if (uiWasVisible) {
        g_state.showUI = true;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Healer boon sequence completed!");
}

void AutomationLogic::ApplyBoons() {
    if (!g_api || !g_state.enabled) return;

    if (g_state.environmentDamage) {
        ApplyHealerBoons();
        return;
    }

    bool uiWasVisible = g_state.showUI;
    if (uiWasVisible) {
        g_state.showUI = false;
    }

    std::string mode = "Normal";
    if (g_state.isQuickDps) {
        mode = "Quick DPS";
    }
    else if (g_state.isAlacDps) {
        mode = "Alac DPS";
    }

    std::string advancedBoons = "";
    if (g_state.showBoonAdvanced && (g_state.addResistance || g_state.addStability || g_state.skipAegis)) {
        advancedBoons = " + ";
        if (g_state.addResistance) advancedBoons += "Resistance ";
        if (g_state.addStability) advancedBoons += "Stability ";
        if (g_state.skipAegis) advancedBoons += "Skip Aegis ";
        advancedBoons.pop_back();
    }

    char startBuffer[300];
    sprintf_s(startBuffer, "Starting boon sequence (20 steps) - Mode: %s%s", mode.c_str(), advancedBoons.c_str());
    g_api->Log(ELogLevel_INFO, "GolemHelper", startBuffer);

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(g_state.initialDelay);

        for (int i = 0; i < MenuSequences::BOON_LENGTH; i++) {
            int stepIndex = i;
            MenuOption step = MenuSequences::BOON_SEQUENCE[i];
            auto coordIt = g_coords.coords.find(step);

            if (coordIt == g_coords.coords.end() ||
                (coordIt->second.first == 0 && coordIt->second.second == 0)) {
                continue;
            }

            if (ShouldSkipBoonStep(stepIndex)) {
                continue;
            }

            if (step == BOON_RESOLUTION) {
                CoordinateUtils::ClickAtScaled(
                    coordIt->second.first,
                    coordIt->second.second,
                    g_state.stepDelay
                );

                if (g_state.showBoonAdvanced && g_state.addResistance) {
                    auto resistanceCoord = g_coords.coords.find(BOON_RESISTANCE);
                    if (resistanceCoord != g_coords.coords.end()) {
                        CoordinateUtils::ClickAtScaled(
                            resistanceCoord->second.first,
                            resistanceCoord->second.second,
                            g_state.stepDelay
                        );
                    }
                }

                if (g_state.showBoonAdvanced && g_state.addStability) {
                    auto stabilityCoord = g_coords.coords.find(BOON_STABILITY);
                    if (stabilityCoord != g_coords.coords.end()) {
                        CoordinateUtils::ClickAtScaled(
                            stabilityCoord->second.first,
                            stabilityCoord->second.second,
                            g_state.stepDelay
                        );
                    }
                }

                continue;
            }

            int delay = (i == MenuSequences::BOON_LENGTH - 1) ? 50 : g_state.stepDelay;
            CoordinateUtils::ClickAtScaled(coordIt->second.first, coordIt->second.second, delay);
        }
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during boon sequence");
    }

    if (uiWasVisible) {
        g_state.showUI = true;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Boon sequence completed!");
}

void AutomationLogic::SpawnGolem() {
    if (!g_api || !g_state.enabled) return;

    bool uiWasVisible = g_state.showUI;
    if (uiWasVisible) {
        g_state.showUI = false;
    }

    const char* hitbox = g_state.hitboxType == HITBOX_SMALL ? "Small Hitbox" :
        g_state.hitboxType == HITBOX_MEDIUM ? "Medium Hitbox" : "Large Hitbox";

    std::string modifiers = "Normal";
    if (g_state.showAdvanced && (g_state.skipBurning || g_state.skipConfusion || g_state.skipSlow ||
        g_state.addImmobilize || g_state.addBlind || g_state.fiveBleedingStacks)) {
        modifiers = "";
        if (g_state.skipBurning) modifiers += "Skip Burning ";
        if (g_state.skipConfusion) modifiers += "Skip Confusion ";
        if (g_state.skipSlow) modifiers += "Skip Slow ";
        if (g_state.addImmobilize) modifiers += "Add Immobilize ";
        if (g_state.addBlind) modifiers += "Add Blind ";
        if (g_state.fiveBleedingStacks) modifiers += "5 Bleeding ";
        if (!modifiers.empty()) modifiers.pop_back();
    }

    char startBuffer[400];
    sprintf_s(startBuffer, "Starting golem settings sequence (25 steps) - Modifiers: %s, Hitbox: %s", modifiers.c_str(), hitbox);
    g_api->Log(ELogLevel_INFO, "GolemHelper", startBuffer);

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(g_state.initialDelay);

        for (int i = 0; i < MenuSequences::GOLEM_LENGTH; i++) {
            int stepIndex = i;
            MenuOption step = MenuSequences::GOLEM_SEQUENCE[i];
            auto coordIt = g_coords.coords.find(step);

            if (coordIt == g_coords.coords.end() ||
                (coordIt->second.first == 0 && coordIt->second.second == 0)) {
                continue;
            }

            if (ShouldSkipGolemStep(stepIndex)) {
                continue;
            }

            int currentX = coordIt->second.first;
            int currentY = coordIt->second.second;

            if (step == GOLEM_HITBOX_SMALL) {
                MenuOption hitboxOption;
                switch (g_state.hitboxType) {
                case HITBOX_SMALL: hitboxOption = GOLEM_HITBOX_SMALL; break;
                case HITBOX_MEDIUM: hitboxOption = GOLEM_HITBOX_MEDIUM; break;
                case HITBOX_LARGE: hitboxOption = GOLEM_HITBOX_LARGE; break;
                default: hitboxOption = GOLEM_HITBOX_SMALL; break;
                }

                auto hitboxCoord = g_coords.coords.find(hitboxOption);
                if (hitboxCoord != g_coords.coords.end()) {
                    currentX = hitboxCoord->second.first;
                    currentY = hitboxCoord->second.second;
                }
            }

            int delay = (i == MenuSequences::GOLEM_LENGTH - 1) ? 50 : g_state.stepDelay;

            if (step == GOLEM_CRIPPLE) {
                CoordinateUtils::ClickAtScaled(currentX, currentY, delay);

                if (g_state.showAdvanced && g_state.addImmobilize) {
                    auto immobilizeCoord = g_coords.coords.find(GOLEM_IMMOBILIZE);
                    if (immobilizeCoord != g_coords.coords.end()) {
                        CoordinateUtils::ClickAtScaled(
                            immobilizeCoord->second.first,
                            immobilizeCoord->second.second,
                            g_state.stepDelay
                        );
                    }
                }

                continue;
            }

            if (step == GOLEM_COMBATAFFECTINGCONDITIONS) {
                CoordinateUtils::ClickAtScaled(currentX, currentY, delay);

                if (g_state.showAdvanced && g_state.addBlind) {
                    auto blindCoord = g_coords.coords.find(GOLEM_BLIND);
                    if (blindCoord != g_coords.coords.end()) {
                        CoordinateUtils::ClickAtScaled(
                            blindCoord->second.first,
                            blindCoord->second.second,
                            g_state.stepDelay
                        );
                    }
                }

                continue;
            }

            CoordinateUtils::ClickAtScaled(currentX, currentY, delay);

            if (g_state.showAdvanced && g_state.fiveBleedingStacks && step == GOLEM_BLEEDING) {
                for (int repeat = 0; repeat < 4; repeat++) {
                    CoordinateUtils::ClickAtScaled(currentX, currentY, g_state.stepDelay);
                }
            }
        }
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during golem settings sequence");
    }

    if (uiWasVisible) {
        g_state.showUI = true;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Golem settings sequence completed (25 steps)!");
}

void AutomationLogic::RespawnGolem() {
    if (!g_api || !g_state.enabled) return;

    bool uiWasVisible = g_state.showUI;
    if (uiWasVisible) {
        g_state.showUI = false;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Starting golem respawn sequence (2 steps)");

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(g_state.initialDelay);

        for (int i = 0; i < MenuSequences::GOLEM_RESPAWN_LENGTH; i++) {
            MenuOption step = MenuSequences::GOLEM_RESPAWN[i];
            auto coordIt = g_coords.coords.find(step);

            if (coordIt == g_coords.coords.end() ||
                (coordIt->second.first == 0 && coordIt->second.second == 0)) {
                continue;
            }

            int delay = (i == MenuSequences::GOLEM_RESPAWN_LENGTH - 1) ? 50 : g_state.stepDelay;
            CoordinateUtils::ClickAtScaled(coordIt->second.first, coordIt->second.second, delay);
        }
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during golem respawn sequence");
    }

    if (uiWasVisible) {
        g_state.showUI = true;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Golem respawn sequence completed!");
}

void AutomationLogic::RemoveAndRespawnGolem()
{
    if (!g_api || !g_state.enabled) return;

    bool uiWasVisible = g_state.showUI;
    if (uiWasVisible) {
        g_state.showUI = false;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Starting golem remove and respawn sequence (3 steps)");

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(g_state.initialDelay);

        for (int i = 0; i < MenuSequences::GOLEM_REMOVE_AND_RESPAWN_LENGTH; i++) {
            MenuOption step = MenuSequences::GOLEM_REMOVE_AND_RESPAWN[i];
            auto coordIt = g_coords.coords.find(step);

            if (coordIt == g_coords.coords.end() ||
                (coordIt->second.first == 0 && coordIt->second.second == 0)) {
                continue;
                }

            int delay = (i == MenuSequences::GOLEM_REMOVE_AND_RESPAWN_LENGTH - 1) ? 50 : g_state.stepDelay;
            CoordinateUtils::ClickAtScaled(coordIt->second.first, coordIt->second.second, delay);
        }
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during golem remove and respawn sequence");
    }

    if (uiWasVisible) {
        g_state.showUI = true;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Golem remove and respawn sequence completed!");
}