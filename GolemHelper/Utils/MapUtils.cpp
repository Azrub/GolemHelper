#include <Windows.h>
#include "MapUtils.h"
#include "../Common/Globals.h"

bool MapUtils::IsInTrainingArea() {
    return GetCurrentMapID() == 1154;
}

unsigned int MapUtils::GetCurrentMapID() {
    if (g_mumbleData && g_mumbleData->Context.MapID != 0) {
        return g_mumbleData->Context.MapID;
    }

    return 0;
}

void MapUtils::UpdateQuickAccessVisibility() {
    if (!g_api) return;

    bool shouldBeVisible = IsInTrainingArea() && !g_state.alwaysHideIcon;
    unsigned int currentMapID = GetCurrentMapID();

    if (currentMapID != g_state.lastMapID || g_state.quickAccessVisible != shouldBeVisible) {
        g_state.lastMapID = currentMapID;

        if (shouldBeVisible && !g_state.quickAccessVisible) {
            g_api->QuickAccess.Add(
                "GolemHelper.ToggleUI",
                "GOLEM_HELPER_ICON",
                "GOLEM_HELPER_ICON_HOVER",
                "GolemHelper.ToggleUI",
                "GolemHelper UI"
            );
            g_state.quickAccessVisible = true;

            if (g_state.debugMode) {
                char buffer[150];
                sprintf_s(buffer, sizeof(buffer), "QuickAccess icon ADDED - MapID: %u (Training Area)", currentMapID);
                g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
            }
        }
        else if ((!shouldBeVisible || g_state.alwaysHideIcon) && g_state.quickAccessVisible) {
            g_api->QuickAccess.Remove("GolemHelper.ToggleUI");
            g_state.quickAccessVisible = false;

            if (g_state.debugMode) {
                const char* reason = g_state.alwaysHideIcon ? "Always Hide Icon enabled" : "Not Training Area";
                char buffer[200];
                sprintf_s(buffer, sizeof(buffer), "QuickAccess icon REMOVED - MapID: %u (%s)", currentMapID, reason);
                g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
            }
        }
    }

    if (g_state.autoShowHideUI) {
        bool inTrainingArea = IsInTrainingArea();
        static bool wasInTrainingArea = false;

        if (inTrainingArea && !wasInTrainingArea) {
            g_state.showUI = true;
        }
        else if (!inTrainingArea && wasInTrainingArea) {
            g_state.showUI = false;
        }

        wasInTrainingArea = inTrainingArea;
    }
}