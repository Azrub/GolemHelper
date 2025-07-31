#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
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

    bool shouldBeVisible = IsInTrainingArea();
    unsigned int currentMapID = GetCurrentMapID();

    if (currentMapID != g_state.lastMapID) {
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
        else if (!shouldBeVisible && g_state.quickAccessVisible) {
            g_api->QuickAccess.Remove("GolemHelper.ToggleUI");
            g_state.quickAccessVisible = false;

            if (g_state.debugMode) {
                char buffer[150];
                sprintf_s(buffer, sizeof(buffer), "QuickAccess icon REMOVED - MapID: %u (Not Training Area)", currentMapID);
                g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
            }
        }
    }
}