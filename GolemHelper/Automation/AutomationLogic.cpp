#include <Windows.h>
#include <string>
#include "AutomationLogic.h"
#include "../Common/Globals.h"
#include "CoordinateUtils.h"

bool AutomationLogic::ShouldSkipBoonStep(int stepIndex) {
    if (g_state.isQuickDps && stepIndex == 14) {
        return true;
    }

    if (g_state.isAlacDps && (stepIndex == 13 || stepIndex == 18)) {
        return true;
    }

    if (g_state.showBoonAdvanced && g_state.skipAegis && stepIndex == 10) {
        return true;
    }

    return false;
}

bool AutomationLogic::ShouldSkipGolemStep(int stepIndex) {
    if (!g_state.showAdvanced) {
        return false;
    }

    if (g_state.skipSlow && stepIndex == 17) {
        return true;
    }

    if (g_state.skipBurning && stepIndex == 7) {
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
        mode += "Quick DPS (Healer provides Alacrity)";
    }
    else if (g_state.isAlacDps) {
        mode += "Alac DPS (Healer provides Quickness)";
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

        if (g_state.isQuickDps) {
            for (int i = 0; i < 10; i++) {
                CoordinateUtils::ClickAtScaled(g_coords.healerStepX[i], g_coords.healerStepY[i], g_state.stepDelay);
            }
        }
        else if (g_state.isAlacDps) {
            int alacStepY[9] = { 262, 352, 352, 305, 500, 450, 450, 305, 262 };
            for (int i = 0; i < 9; i++) {
                CoordinateUtils::ClickAtScaled(830, alacStepY[i], g_state.stepDelay);
            }
        }

        g_api->Log(ELogLevel_INFO, "GolemHelper", "Applying Environment Damage final click");

        int finalY;
        switch (g_state.envDamageLevel) {
        case ENV_MILD: finalY = 352; break;
        case ENV_MODERATE: finalY = 305; break;
        case ENV_EXTREME: finalY = 262; break;
        default: finalY = 352; break;
        }

        CoordinateUtils::ClickAtScaled(830, finalY, 50);
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during healer boon sequence");
    }

    if (uiWasVisible) {
        g_state.showUI = true;
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Healer boon sequence completed!");
}

void AutomationLogic::ApplyAllBoons() {
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

        for (int i = 0; i < 20; i++) {
            if (g_coords.boonStepX[i] == 0 && g_coords.boonStepY[i] == 0) {
                continue;
            }

            if (ShouldSkipBoonStep(i)) {
                continue;
            }

            if (i == 9) {
                CoordinateUtils::ClickAtScaled(g_coords.boonStepX[i], g_coords.boonStepY[i], g_state.stepDelay);

                if (g_state.showBoonAdvanced && g_state.addResistance) {
                    g_api->Log(ELogLevel_INFO, "GolemHelper", "Adding Resistance");
                    CoordinateUtils::ClickAtScaled(g_coords.resistanceX, g_coords.resistanceY, g_state.stepDelay);
                }

                if (g_state.showBoonAdvanced && g_state.addStability) {
                    g_api->Log(ELogLevel_INFO, "GolemHelper", "Adding Stability");
                    CoordinateUtils::ClickAtScaled(g_coords.stabilityX, g_coords.stabilityY, g_state.stepDelay);
                }

                continue;
            }

            int delay = (i == 19) ? 50 : g_state.stepDelay;
            CoordinateUtils::ClickAtScaled(g_coords.boonStepX[i], g_coords.boonStepY[i], delay);
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

void AutomationLogic::ApplyGolemSettings() {
    if (!g_api || !g_state.enabled) return;

    bool uiWasVisible = g_state.showUI;
    if (uiWasVisible) {
        g_state.showUI = false;
    }

    const char* hitbox = g_state.hitboxType == HITBOX_SMALL ? "Small Hitbox" :
        g_state.hitboxType == HITBOX_MEDIUM ? "Medium Hitbox" : "Large Hitbox";

    std::string modifiers = "Normal";
    if (g_state.showAdvanced && (g_state.skipSlow || g_state.skipBurning || g_state.fiveBleedingStacks)) {
        modifiers = "";
        if (g_state.skipSlow) modifiers += "Skip Slow ";
        if (g_state.skipBurning) modifiers += "Skip Burning ";
        if (g_state.fiveBleedingStacks) modifiers += "5 Bleeding ";
        if (!modifiers.empty()) modifiers.pop_back();
    }

    char startBuffer[400];
    sprintf_s(startBuffer, "Starting golem settings sequence (25 steps) - Modifiers: %s, Hitbox: %s", modifiers.c_str(), hitbox);
    g_api->Log(ELogLevel_INFO, "GolemHelper", startBuffer);

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(g_state.initialDelay);

        for (int i = 0; i < 25; i++) {
            if (g_coords.golemStepX[i] == 0 && g_coords.golemStepY[i] == 0) {
                continue;
            }

            if (ShouldSkipGolemStep(i)) {
                continue;
            }

            int currentX = g_coords.golemStepX[i];
            int currentY = g_coords.golemStepY[i];

            if (i == 1) {
                switch (g_state.hitboxType) {
                case HITBOX_SMALL:
                    currentY = 260;
                    break;
                case HITBOX_MEDIUM:
                    currentY = 305;
                    break;
                case HITBOX_LARGE:
                    currentY = 352;
                    break;
                }
            }

            int delay = (i == 24) ? 50 : g_state.stepDelay;

            CoordinateUtils::ClickAtScaled(currentX, currentY, delay);

            if (g_state.showAdvanced && g_state.fiveBleedingStacks && i == 6) {
                g_api->Log(ELogLevel_INFO, "GolemHelper", "5 Bleeding Stacks - repeating 7th step 4 more times");

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