#include "pch.h"
#include "Definitions/Nexus.h"
#include <Windows.h>
#include <vector>
#include <string>

AddonAPI* g_api = nullptr;

struct {
    bool enabled = false;
    bool boonsEnabled = true;
    bool golemEnabled = true;
    bool isQuickDps = false;
    bool isAlacDps = false;
    bool isChronomancer = false;
    bool debugMode = false;
    int debugCounter = 0;
} g_state;

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
} g_coords;

void GetScaledCoordinates(int baseX, int baseY, int* scaledX, int* scaledY) {
    g_api->Log(ELogLevel_INFO, "GolemHelper", "GetScaledCoordinates CHIAMATA");

    NexusLinkData* nexusData = (NexusLinkData*)g_api->DataLink.Get("DL_NEXUS_LINK");

    if (nexusData && nexusData->Width > 0 && nexusData->Height > 0) {
        float uiScale = nexusData->Scaling;
        float dpiScale = (float)nexusData->Width / 1920.0f;

        char valuesBuffer[200];
        sprintf_s(valuesBuffer, "GetScaled INPUT: uiScale=%.3f, base=%d,%d", uiScale, baseX, baseY);
        g_api->Log(ELogLevel_INFO, "GolemHelper", valuesBuffer);

        int scaledForResolutionX = (int)(baseX * dpiScale);
        int scaledForResolutionY = baseY;

        int finalX = scaledForResolutionX;
        int finalY = scaledForResolutionY;

        if (uiScale >= 0.89f && uiScale <= 0.91f) {
            finalX = scaledForResolutionX - (int)(scaledForResolutionX * 0.029f);
            finalY = scaledForResolutionY - (int)(scaledForResolutionY * 0.103f);
            g_api->Log(ELogLevel_INFO, "GolemHelper", "APPLIED SMALL OFFSET");
        }
        else if (uiScale >= 1.09f && uiScale <= 1.15f) {
            finalX = scaledForResolutionX - (int)(scaledForResolutionX * 0.053f);
            finalY = scaledForResolutionY + (int)(scaledForResolutionY * 0.095f);
            g_api->Log(ELogLevel_INFO, "GolemHelper", "APPLIED LARGE OFFSET");
        }
        else if (uiScale >= 1.21f && uiScale <= 1.25f) {
            finalX = scaledForResolutionX - (int)(scaledForResolutionX * 0.097f);
            finalY = scaledForResolutionY + (int)(scaledForResolutionY * 0.206f);
            g_api->Log(ELogLevel_INFO, "GolemHelper", "APPLIED LARGER OFFSET");
        }
        else {
            char buffer[100];
            sprintf_s(buffer, "NO OFFSET - uiScale %.3f", uiScale);
            g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
        }

        *scaledX = finalX;
        *scaledY = finalY;

        char resultBuffer[200];
        sprintf_s(resultBuffer, "GetScaled RESULT: %d,%d -> %d,%d", scaledForResolutionX, scaledForResolutionY, finalX, finalY);
        g_api->Log(ELogLevel_INFO, "GolemHelper", resultBuffer);
    }
    else {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "GetScaledCoordinates - Nexus data not available");

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        float dpiScale = (float)screenWidth / 1920.0f;

        *scaledX = (int)(baseX * dpiScale);
        *scaledY = baseY;
    }
}

void DebugMousePosition() {
    if (!g_api) return;

    POINT mousePos;
    GetCursorPos(&mousePos);

    NexusLinkData* nexusData = (NexusLinkData*)g_api->DataLink.Get("DL_NEXUS_LINK");

    if (nexusData && nexusData->Width > 0 && nexusData->Height > 0) {
        float uiScale = nexusData->Scaling;
        float dpiScale = (float)nexusData->Width / 1920.0f;
        float finalScaleX = uiScale * dpiScale;

        int baseX = (int)(mousePos.x / finalScaleX);
        int baseY = mousePos.y;

        g_state.debugCounter++;

        char buffer[450];
        sprintf_s(buffer, "=== DEBUG #%d === Resolution: %dx%d | Mouse: %d,%d | Base coords: %d,%d | Interface Size: %.2f | DPI Scale: %.3f | Final ScaleX: %.3f",
            g_state.debugCounter, nexusData->Width, nexusData->Height,
            mousePos.x, mousePos.y, baseX, baseY, uiScale, dpiScale, finalScaleX);
        g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
    }
    else {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Cannot debug - Nexus data not available");
    }

    if (g_state.debugCounter == 1) {
        g_api->UI.SendAlert("Debug active - Interface Size + DPI scaling");
    }
}

void ClickAtScaled(int baseX, int baseY, int delay = 25) {
    HWND gameWindow = GetForegroundWindow();
    if (!gameWindow) return;

    int scaledX, scaledY;
    GetScaledCoordinates(baseX, baseY, &scaledX, &scaledY);

    LPARAM lParam = MAKELPARAM(scaledX, scaledY);
    SendMessage(gameWindow, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    Sleep(10);
    SendMessage(gameWindow, WM_LBUTTONUP, 0, lParam);
    Sleep(delay);
}

bool ShouldSkipBoonStep(int stepIndex) {
    if (g_state.isQuickDps && stepIndex == 14) {
        return true;
    }

    if (g_state.isAlacDps && (stepIndex == 13 || stepIndex == 18)) {
        return true;
    }

    return false;
}

bool ShouldSkipGolemStep(int stepIndex) {
    if (g_state.isChronomancer && stepIndex == 17) {
        return true;
    }

    return false;
}

void ApplyAllBoons() {
    if (!g_api || !g_state.boonsEnabled || !g_state.enabled) return;

    NexusLinkData* nexusData = (NexusLinkData*)g_api->DataLink.Get("DL_NEXUS_LINK");
    if (nexusData && nexusData->Width > 0 && nexusData->Height > 0) {
        float uiScale = nexusData->Scaling;
        float dpiScale = (float)nexusData->Width / 1920.0f;
        float finalScaleX = dpiScale;

        char scalingBuffer[300];
        sprintf_s(scalingBuffer, "Current scaling - Resolution: %dx%d | Interface Size: %.3f | DPI Scale: %.3f | Final ScaleX: %.3f",
            nexusData->Width, nexusData->Height, uiScale, dpiScale, finalScaleX);
        g_api->Log(ELogLevel_INFO, "GolemHelper", scalingBuffer);

        if (uiScale >= 0.89f && uiScale <= 0.91f) {
            g_api->Log(ELogLevel_INFO, "GolemHelper", "Applying SMALL interface offset");
        }
        else if (uiScale >= 1.09f && uiScale <= 1.15f) {
            g_api->Log(ELogLevel_INFO, "GolemHelper", "Applying LARGE interface offset");
        }
        else if (uiScale >= 1.21f && uiScale <= 1.25f) {
            g_api->Log(ELogLevel_INFO, "GolemHelper", "Applying LARGER interface offset");
        }
        else {
            char noOffsetBuffer[150];
            sprintf_s(noOffsetBuffer, "NO OFFSET applied - uiScale %.3f doesn't match any range", uiScale);
            g_api->Log(ELogLevel_INFO, "GolemHelper", noOffsetBuffer);
        }
    }

    const char* mode = g_state.isQuickDps ? "Quick DPS" :
        g_state.isAlacDps ? "Alac DPS" : "Normal";

    char startBuffer[200];
    sprintf_s(startBuffer, "Starting boon sequence (20 steps) - Mode: %s", mode);
    g_api->Log(ELogLevel_INFO, "GolemHelper", startBuffer);

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(390);

        for (int i = 0; i < 20; i++) {
            if (g_coords.boonStepX[i] == 0 && g_coords.boonStepY[i] == 0) {
                char skipBuffer[100];
                sprintf_s(skipBuffer, "Skipping boon step %d (coordinates 0,0)", i + 1);
                g_api->Log(ELogLevel_INFO, "GolemHelper", skipBuffer);
                continue;
            }

            if (ShouldSkipBoonStep(i)) {
                char skipBuffer[150];
                sprintf_s(skipBuffer, "Skipping boon step %d (%s mode) - coordinates: %d,%d",
                    i + 1, mode, g_coords.boonStepX[i], g_coords.boonStepY[i]);
                g_api->Log(ELogLevel_INFO, "GolemHelper", skipBuffer);
                continue;
            }

            char stepBuffer[200];
            sprintf_s(stepBuffer, "Executing boon step %d at base coordinates: %d,%d",
                i + 1, g_coords.boonStepX[i], g_coords.boonStepY[i]);
            g_api->Log(ELogLevel_INFO, "GolemHelper", stepBuffer);

            int scaledX, scaledY;
            GetScaledCoordinates(g_coords.boonStepX[i], g_coords.boonStepY[i], &scaledX, &scaledY);
            char scaledBuffer[200];
            sprintf_s(scaledBuffer, "-> FINAL SCALED COORDINATES: %d,%d", scaledX, scaledY);
            g_api->Log(ELogLevel_INFO, "GolemHelper", scaledBuffer);

            int delay = (i == 19) ? 50 : 290;
            ClickAtScaled(g_coords.boonStepX[i], g_coords.boonStepY[i], delay);
        }
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during boon sequence");
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Boon sequence completed!");
}

void ApplyGolemSettings() {
    if (!g_api || !g_state.golemEnabled || !g_state.enabled) return;

    NexusLinkData* nexusData = (NexusLinkData*)g_api->DataLink.Get("DL_NEXUS_LINK");
    if (nexusData && nexusData->Width > 0 && nexusData->Height > 0) {
        float uiScale = nexusData->Scaling;
        float dpiScale = (float)nexusData->Width / 1920.0f;
        float finalScaleX = dpiScale;

        char scalingBuffer[300];
        sprintf_s(scalingBuffer, "Current scaling - Resolution: %dx%d | Interface Size: %.3f | DPI Scale: %.3f | Final ScaleX: %.3f",
            nexusData->Width, nexusData->Height, uiScale, dpiScale, finalScaleX);
        g_api->Log(ELogLevel_INFO, "GolemHelper", scalingBuffer);
    }

    const char* mode = g_state.isChronomancer ? "Chronomancer" : "Normal";
    char startBuffer[200];
    sprintf_s(startBuffer, "Starting golem settings sequence (25 steps) - Mode: %s", mode);
    g_api->Log(ELogLevel_INFO, "GolemHelper", startBuffer);

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        Sleep(390);

        for (int i = 0; i < 25; i++) {
            if (g_coords.golemStepX[i] == 0 && g_coords.golemStepY[i] == 0) {
                char skipBuffer[100];
                sprintf_s(skipBuffer, "Skipping golem step %d (coordinates 0,0)", i + 1);
                g_api->Log(ELogLevel_INFO, "GolemHelper", skipBuffer);
                continue;
            }

            if (ShouldSkipGolemStep(i)) {
                char skipBuffer[150];
                sprintf_s(skipBuffer, "Skipping golem step %d (%s mode) - coordinates: %d,%d",
                    i + 1, mode, g_coords.golemStepX[i], g_coords.golemStepY[i]);
                g_api->Log(ELogLevel_INFO, "GolemHelper", skipBuffer);
                continue;
            }

            char stepBuffer[150];
            sprintf_s(stepBuffer, "Executing golem step %d at base coordinates: %d,%d",
                i + 1, g_coords.golemStepX[i], g_coords.golemStepY[i]);
            g_api->Log(ELogLevel_INFO, "GolemHelper", stepBuffer);

            int delay = (i == 24) ? 50 : 290;
            ClickAtScaled(g_coords.golemStepX[i], g_coords.golemStepY[i], delay);
        }
    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Exception during golem settings sequence");
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "Golem settings sequence completed (25 steps)!");
}

void HandleBoonKeybind(const char* id, bool release) {
    if (!release && g_state.enabled) ApplyAllBoons();
}

void HandleGolemKeybind(const char* id, bool release) {
    if (!release && g_state.enabled) ApplyGolemSettings();
}

void HandleToggleKeybind(const char* id, bool release) {
    if (!release) {
        g_state.enabled = !g_state.enabled;
        g_api->UI.SendAlert(g_state.enabled ? "GolemHelper enabled" : "GolemHelper disabled");
    }
}

void HandleDebugKeybind(const char* id, bool release) {
    if (!release) DebugMousePosition();
}

void HandleQuickDpsKeybind(const char* id, bool release) {
    if (!release) {
        g_state.isQuickDps = !g_state.isQuickDps;
        if (g_state.isQuickDps) {
            g_state.isAlacDps = false;
        }
        g_api->UI.SendAlert(g_state.isQuickDps ? "Quick DPS mode enabled" : "Quick DPS mode disabled");
        g_api->Log(ELogLevel_INFO, "GolemHelper", g_state.isQuickDps ? "Quick DPS enabled" : "Quick DPS disabled");
    }
}

void HandleAlacDpsKeybind(const char* id, bool release) {
    if (!release) {
        g_state.isAlacDps = !g_state.isAlacDps;
        if (g_state.isAlacDps) {
            g_state.isQuickDps = false;
        }
        g_api->UI.SendAlert(g_state.isAlacDps ? "Alac DPS mode enabled" : "Alac DPS mode disabled");
        g_api->Log(ELogLevel_INFO, "GolemHelper", g_state.isAlacDps ? "Alac DPS enabled" : "Alac DPS disabled");
    }
}

void HandleChronomancerKeybind(const char* id, bool release) {
    if (!release) {
        g_state.isChronomancer = !g_state.isChronomancer;
        g_api->UI.SendAlert(g_state.isChronomancer ? "Chronomancer mode enabled" : "Chronomancer mode disabled");
        g_api->Log(ELogLevel_INFO, "GolemHelper", g_state.isChronomancer ? "Chronomancer enabled" : "Chronomancer disabled");
    }
}

void Load(AddonAPI* api) {
    g_api = api;
    g_state.enabled = true;

    Keybind kb_empty = { 0, false, false, false };
    g_api->InputBinds.RegisterWithStruct("GolemHelper.ApplyBoons", HandleBoonKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.ApplyGolem", HandleGolemKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.QuickDPS", HandleQuickDpsKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.AlacDPS", HandleAlacDpsKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.Chronomancer", HandleChronomancerKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.Toggle", HandleToggleKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.DebugMouse", HandleDebugKeybind, kb_empty);

    g_api->Log(ELogLevel_INFO, "GolemHelper", "=== GolemHelper v1.0.0-beta.1 Loaded with Array System + DPS Modes + Chronomancer ===");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "Available keybinds:");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "Available keybinds:");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "- ApplyBoons: Apply boons with current DPS mode");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "- ApplyGolem: Apply golem settings");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "- QuickDPS: Toggle Quick DPS mode (skip quickness)");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "- AlacDPS: Toggle Alac DPS mode (skip alacrity)");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "- Chronomancer: Toggle Chronomancer mode (skip slow on golem)");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "- Toggle: Enable/disable addon");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "- DebugMouse: Show mouse position (for coordinate mapping)");

    const char* boonMode = g_state.isQuickDps ? "Quick DPS" :
        g_state.isAlacDps ? "Alac DPS" : "Normal";
    const char* golemMode = g_state.isChronomancer ? "Chronomancer" : "Normal";

    char modeBuffer[150];
    sprintf_s(modeBuffer, "Current Modes - Boons: %s | Golem: %s | Base coordinates: 1080p", boonMode, golemMode);
    g_api->Log(ELogLevel_INFO, "GolemHelper", modeBuffer);
}

void Unload() {
    g_api->InputBinds.Deregister("GolemHelper.ApplyBoons");
    g_api->InputBinds.Deregister("GolemHelper.ApplyGolem");
    g_api->InputBinds.Deregister("GolemHelper.QuickDPS");
    g_api->InputBinds.Deregister("GolemHelper.AlacDPS");
    g_api->InputBinds.Deregister("GolemHelper.Chronomancer");
    g_api->InputBinds.Deregister("GolemHelper.Toggle");
    g_api->InputBinds.Deregister("GolemHelper.DebugMouse");
    g_api = nullptr;
    g_state.enabled = false;
}

extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef() {
    static AddonDefinition def;
    def.Signature = -424248;
    def.APIVersion = NEXUS_API_VERSION;
    def.Name = "GolemHelper";
    def.Version = { 1, 0, 0, 1 };
    def.Author = "Azrub";
    def.Description = "Boon & golem automation with DPS modes, Chronomancer mode and auto-scaling [BETA]";
    def.Load = Load;
    def.Unload = Unload;
    def.Flags = EAddonFlags_None;
    def.Provider = EUpdateProvider_GitHub;
    def.UpdateLink = "https://github.com/Azrub/GolemHelper";
    return &def;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }