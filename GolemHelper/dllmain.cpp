#include "pch.h"
#include "Definitions/Nexus.h"
#include <Windows.h>
#include <vector>
#include <string>

#include "Dependencies/imgui/imgui.h"
#include "Dependencies/mumble/Mumble.h"

AddonAPI* g_api = nullptr;
NexusLinkData* NexusLink = nullptr;

enum HitboxType {
    HITBOX_SMALL = 0,
    HITBOX_MEDIUM = 1,
    HITBOX_BIG = 2
};

enum EnvironmentDamageLevel {
    ENV_MILD = 0,
    ENV_MODERATE = 1,
    ENV_EXTREME = 2
};

struct {
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
    int debugCounter = 0;

    int stepDelay = 290;
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

void GetScaledCoordinates(int baseX, int baseY, int* scaledX, int* scaledY);
void DebugMousePosition();
void ApplyAllBoons();
void ApplyGolemSettings();
void ClickAtScaled(int baseX, int baseY, int delay);
bool ShouldSkipBoonStep(int stepIndex);
bool ShouldSkipGolemStep(int stepIndex);
void HandleBoonKeybind(const char* id, bool release);
void HandleGolemKeybind(const char* id, bool release);
void HandleQuickDpsKeybind(const char* id, bool release);
void HandleAlacDpsKeybind(const char* id, bool release);
void HandleToggleKeybind(const char* id, bool release);
void HandleUIToggleKeybind(const char* id, bool release);
void HandleDebugKeybind(const char* id, bool release);
void Load(AddonAPI* aApi);
void Unload();

void RenderUI() {
    if (!g_state.showUI) return;

    ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("GolemHelper", &g_state.showUI, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "GolemHelper v1.0.0.0");
        ImGui::Separator();

        ImGui::Text("Status:");
        ImGui::SameLine();
        if (g_state.enabled) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "ENABLED");
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "DISABLED");
        }

        if (ImGui::Button(g_state.enabled ? "Disable GolemHelper" : "Enable GolemHelper", ImVec2(200, 0))) {
            g_state.enabled = !g_state.enabled;
        }

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("Boon Configuration");
        ImGui::Checkbox("Enable Boons", &g_state.boonsEnabled);

        if (ImGui::Button("Apply Boons", ImVec2(150, 0))) {
            if (g_state.enabled) {
                g_api->InputBinds.Invoke("GolemHelper.ApplyBoons", false);
            }
        }

        ImGui::Text("DPS Modes:");

        if (ImGui::RadioButton("Normal Mode", !g_state.isQuickDps && !g_state.isAlacDps)) {
            g_state.isQuickDps = false;
            g_state.isAlacDps = false;
        }

        if (ImGui::RadioButton("Quick DPS (Skip Quickness)", g_state.isQuickDps)) {
            g_state.isQuickDps = true;
            g_state.isAlacDps = false;
        }

        if (ImGui::RadioButton("Alac DPS (Skip Alacrity)", g_state.isAlacDps)) {
            g_state.isQuickDps = false;
            g_state.isAlacDps = true;
        }

        ImGui::Spacing();
        ImGui::Text("Healer Bench:");

        ImGui::Checkbox("Environment Damage", &g_state.environmentDamage);

        if (g_state.environmentDamage) {
            if (ImGui::RadioButton("Mild", g_state.envDamageLevel == ENV_MILD)) {
                g_state.envDamageLevel = ENV_MILD;
            }

            if (ImGui::RadioButton("Moderate", g_state.envDamageLevel == ENV_MODERATE)) {
                g_state.envDamageLevel = ENV_MODERATE;
            }

            if (ImGui::RadioButton("Extreme", g_state.envDamageLevel == ENV_EXTREME)) {
                g_state.envDamageLevel = ENV_EXTREME;
            }
        }

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("Golem Configuration");
        ImGui::Checkbox("Enable Golem Settings", &g_state.golemEnabled);

        if (ImGui::Button("Apply Golem Settings", ImVec2(150, 0))) {
            if (g_state.enabled) {
                g_api->InputBinds.Invoke("GolemHelper.ApplyGolem", false);
            }
        }

        ImGui::Text("Golem Hitbox:");

        if (ImGui::RadioButton("Small (Benchmark Default)", g_state.hitboxType == HITBOX_SMALL)) {
            g_state.hitboxType = HITBOX_SMALL;
        }

        if (ImGui::RadioButton("Medium", g_state.hitboxType == HITBOX_MEDIUM)) {
            g_state.hitboxType = HITBOX_MEDIUM;
        }

        if (ImGui::RadioButton("Big", g_state.hitboxType == HITBOX_BIG)) {
            g_state.hitboxType = HITBOX_BIG;
        }

        ImGui::Spacing();
        ImGui::Text("Advanced:");

        ImGui::Checkbox("Condition Settings", &g_state.showAdvanced);

        if (g_state.showAdvanced) {
            ImGui::Checkbox("Skip Slow", &g_state.skipSlow);
            ImGui::Checkbox("Skip Burning", &g_state.skipBurning);
            ImGui::Checkbox("5 Bleeding Stacks", &g_state.fiveBleedingStacks);
        }

        if (g_state.debugMode) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Text("Display Info:");
            if (NexusLink && NexusLink->Width > 0 && NexusLink->Height > 0) {
                ImGui::Text("Resolution: %dx%d", NexusLink->Width, NexusLink->Height);
                ImGui::Text("UI Scale: %.2f", NexusLink->Scaling);
                float dpiScale = (float)NexusLink->Width / 1920.0f;
                ImGui::Text("DPI Scale: %.3f", dpiScale);
                ImGui::Text("Debug samples: %d", g_state.debugCounter);
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Timing Settings");

        bool wasUsingCustom = g_state.useCustomDelays;
        ImGui::Checkbox("Use Custom Step Delay", &g_state.useCustomDelays);

        if (g_state.useCustomDelays) {
            ImGui::SliderInt("", &g_state.stepDelay, 100, 1000, "%d ms");

            ImGui::Spacing();
            if (ImGui::Button("Reset to Default", ImVec2(120, 0))) {
                g_state.stepDelay = 290;
            }
            ImGui::SameLine();
            if (ImGui::Button("Slow Mode", ImVec2(80, 0))) {
                g_state.stepDelay = 1000;
            }

            if (!wasUsingCustom) {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "Increase delay if clicks fail");
            }
        }
    }

    ImGui::End();
}

void RenderOptions() {
    ImGui::Separator();
    ImGui::Text("GolemHelper Settings");

    ImGui::Checkbox("Show UI by default", &g_state.showUI);
    ImGui::Checkbox("Enable debug mode", &g_state.debugMode);

    if (ImGui::Button("Reset all settings")) {
        g_state.isQuickDps = false;
        g_state.isAlacDps = false;
        g_state.environmentDamage = false;
        g_state.envDamageLevel = ENV_MILD;
        g_state.skipSlow = false;
        g_state.skipBurning = false;
        g_state.fiveBleedingStacks = false;
        g_state.hitboxType = HITBOX_SMALL;
        g_state.useCustomDelays = false;
        g_state.showAdvanced = false;
        g_state.stepDelay = 290;
        g_state.boonsEnabled = true;
        g_state.golemEnabled = true;
    }

    ImGui::Spacing();
    ImGui::Text("Current Modes:");

    std::string boonMode = "Normal";
    if (g_state.isQuickDps) {
        boonMode = "Quick DPS";
    }
    else if (g_state.isAlacDps) {
        boonMode = "Alac DPS";
    }

    if (g_state.environmentDamage) {
        boonMode += " + Env ";
        switch (g_state.envDamageLevel) {
        case ENV_MILD: boonMode += "Mild"; break;
        case ENV_MODERATE: boonMode += "Moderate"; break;
        case ENV_EXTREME: boonMode += "Extreme"; break;
        }
    }

    ImGui::Text("- Boons: %s", boonMode.c_str());

    std::string golemMods = "Normal";
    if (g_state.showAdvanced && (g_state.skipSlow || g_state.skipBurning || g_state.fiveBleedingStacks)) {
        golemMods = "";
        if (g_state.skipSlow) golemMods += "Skip Slow ";
        if (g_state.skipBurning) golemMods += "Skip Burning ";
        if (g_state.fiveBleedingStacks) golemMods += "5 Bleeding ";
        if (!golemMods.empty()) golemMods.pop_back();
    }
    ImGui::Text("- Golem: %s", golemMods.c_str());

    const char* hitboxName = g_state.hitboxType == HITBOX_SMALL ? "Small" :
        g_state.hitboxType == HITBOX_MEDIUM ? "Medium" : "Big";
    ImGui::Text("- Hitbox: %s", hitboxName);

    ImGui::Text("- Timing: %s", g_state.useCustomDelays ? "Custom" : "Default");
}

void GetScaledCoordinates(int baseX, int baseY, int* scaledX, int* scaledY) {
    g_api->Log(ELogLevel_INFO, "GolemHelper", "GetScaledCoordinates CHIAMATA");

    if (NexusLink && NexusLink->Width > 0 && NexusLink->Height > 0) {
        float uiScale = NexusLink->Scaling;
        float dpiScale = (float)NexusLink->Width / 1920.0f;

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

    if (NexusLink && NexusLink->Width > 0 && NexusLink->Height > 0) {
        float uiScale = NexusLink->Scaling;
        float dpiScale = (float)NexusLink->Width / 1920.0f;
        float finalScaleX = uiScale * dpiScale;

        int baseX = (int)(mousePos.x / finalScaleX);
        int baseY = mousePos.y;

        g_state.debugCounter++;

        char buffer[450];
        sprintf_s(buffer, "=== DEBUG #%d === Resolution: %dx%d | Mouse: %d,%d | Base coords: %d,%d | Interface Size: %.2f | DPI Scale: %.3f | Final ScaleX: %.3f",
            g_state.debugCounter, NexusLink->Width, NexusLink->Height,
            mousePos.x, mousePos.y, baseX, baseY, uiScale, dpiScale, finalScaleX);
        g_api->Log(ELogLevel_INFO, "GolemHelper", buffer);
    }
    else {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Cannot debug - Nexus data not available");
    }

    if (g_state.debugCounter == 1) {
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

void ApplyAllBoons() {
    if (!g_api || !g_state.boonsEnabled || !g_state.enabled) return;

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

    if (g_state.environmentDamage) {
        mode += " + Environment ";
        switch (g_state.envDamageLevel) {
        case ENV_MILD: mode += "Mild"; break;
        case ENV_MODERATE: mode += "Moderate"; break;
        case ENV_EXTREME: mode += "Extreme"; break;
        }
    }

    char startBuffer[300];
    sprintf_s(startBuffer, "Starting boon sequence (20 steps) - Mode: %s", mode.c_str());
    g_api->Log(ELogLevel_INFO, "GolemHelper", startBuffer);

    try {
        g_api->GameBinds.InvokeAsync(EGameBinds_MiscInteract, 50);
        int initialDelay = g_state.useCustomDelays ? g_state.stepDelay : 390;
        Sleep(initialDelay);

        int maxSteps = g_state.environmentDamage ? 19 : 20;

        for (int i = 0; i < maxSteps; i++) {
            if (g_coords.boonStepX[i] == 0 && g_coords.boonStepY[i] == 0) {
                continue;
            }

            if (ShouldSkipBoonStep(i)) {
                continue;
            }

            int delay = (i == maxSteps - 1) ? 50 : (g_state.useCustomDelays ? g_state.stepDelay : 290);

            ClickAtScaled(g_coords.boonStepX[i], g_coords.boonStepY[i], delay);
        }

        if (g_state.environmentDamage) {
            g_api->Log(ELogLevel_INFO, "GolemHelper", "Applying Environment Damage sequence");

            int delay = g_state.useCustomDelays ? g_state.stepDelay : 290;

            Sleep(delay);
            ClickAtScaled(830, 500, delay);
            ClickAtScaled(830, 454, delay);
            ClickAtScaled(830, 454, delay);
            ClickAtScaled(830, 306, delay);
            ClickAtScaled(830, 257, delay);

            int finalY;
            switch (g_state.envDamageLevel) {
            case ENV_MILD: finalY = 352; break;
            case ENV_MODERATE: finalY = 306; break;
            case ENV_EXTREME: finalY = 257; break;
            default: finalY = 352; break;
            }

            ClickAtScaled(830, finalY, 50);
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

void ApplyGolemSettings() {
    if (!g_api || !g_state.golemEnabled || !g_state.enabled) return;

    bool uiWasVisible = g_state.showUI;
    if (uiWasVisible) {
        g_state.showUI = false;
    }

    const char* hitbox = g_state.hitboxType == HITBOX_SMALL ? "Small Hitbox" :
        g_state.hitboxType == HITBOX_MEDIUM ? "Medium Hitbox" : "Big Hitbox";

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
        int initialDelay = g_state.useCustomDelays ? g_state.stepDelay : 390;
        Sleep(initialDelay);

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
                case HITBOX_BIG:
                    currentY = 352;
                    break;
                }
            }

            int delay = (i == 24) ? 50 : (g_state.useCustomDelays ? g_state.stepDelay : 290);

            ClickAtScaled(currentX, currentY, delay);

            if (g_state.showAdvanced && g_state.fiveBleedingStacks && i == 6) {
                g_api->Log(ELogLevel_INFO, "GolemHelper", "5 Bleeding Stacks - repeating 7th step 4 more times");

                for (int repeat = 0; repeat < 4; repeat++) {
                    int repeatDelay = g_state.useCustomDelays ? g_state.stepDelay : 290;
                    ClickAtScaled(currentX, currentY, repeatDelay);
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

void HandleBoonKeybind(const char* id, bool release) {
    if (!release && g_state.enabled) {
        ApplyAllBoons();
    }
}

void HandleGolemKeybind(const char* id, bool release) {
    if (!release && g_state.enabled) {
        ApplyGolemSettings();
    }
}

void HandleToggleKeybind(const char* id, bool release) {
    if (!release) {
        g_state.enabled = !g_state.enabled;
        g_api->UI.SendAlert(g_state.enabled ? "GolemHelper enabled" : "GolemHelper disabled");
    }
}

void HandleUIToggleKeybind(const char* id, bool release) {
    if (!release) {
        g_state.showUI = !g_state.showUI;
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
    }
}

void HandleAlacDpsKeybind(const char* id, bool release) {
    if (!release) {
        g_state.isAlacDps = !g_state.isAlacDps;
        if (g_state.isAlacDps) {
            g_state.isQuickDps = false;
        }
        g_api->UI.SendAlert(g_state.isAlacDps ? "Alac DPS mode enabled" : "Alac DPS mode disabled");
    }
}

void Load(AddonAPI* aApi) {
    g_api = aApi;

    ImGui::SetCurrentContext((ImGuiContext*)g_api->ImguiContext);
    ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))g_api->ImguiMalloc, (void(*)(void*, void*))g_api->ImguiFree);

    NexusLink = (NexusLinkData*)g_api->DataLink.Get("DL_NEXUS_LINK");

    g_state.enabled = true;

    g_api->Renderer.Register(ERenderType_Render, RenderUI);
    g_api->Renderer.Register(ERenderType_OptionsRender, RenderOptions);

    Keybind kb_empty = { 0, false, false, false };
    g_api->InputBinds.RegisterWithStruct("GolemHelper.ApplyBoons", HandleBoonKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.ApplyGolem", HandleGolemKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.QuickDPS", HandleQuickDpsKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.AlacDPS", HandleAlacDpsKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.Toggle", HandleToggleKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.ToggleUI", HandleUIToggleKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.DebugMouse", HandleDebugKeybind, kb_empty);

    g_api->Log(ELogLevel_INFO, "GolemHelper", "=== GolemHelper v1.0.0.0 Loaded ===");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "<c=#00ff00>GolemHelper addon</c> loaded successfully!");
}

void Unload() {
    if (g_api) {
        g_api->Renderer.Deregister(RenderUI);
        g_api->Renderer.Deregister(RenderOptions);
        g_api->InputBinds.Deregister("GolemHelper.ApplyBoons");
        g_api->InputBinds.Deregister("GolemHelper.ApplyGolem");
        g_api->InputBinds.Deregister("GolemHelper.QuickDPS");
        g_api->InputBinds.Deregister("GolemHelper.AlacDPS");
        g_api->InputBinds.Deregister("GolemHelper.Toggle");
        g_api->InputBinds.Deregister("GolemHelper.ToggleUI");
        g_api->InputBinds.Deregister("GolemHelper.DebugMouse");
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "<c=#ff0000>GolemHelper signing off</c>, it was an honor commander.");
    g_api = nullptr;
    g_state.enabled = false;
    g_state.showUI = false;
}

extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef() {
    static AddonDefinition def;
    def.Signature = -424248;
    def.APIVersion = NEXUS_API_VERSION;
    def.Name = "GolemHelper";
    def.Version = { 1, 0, 0, 0 };
    def.Author = "Azrub";
    def.Description = "Automatically applies boons and golem settings in the training area";
    def.Load = Load;
    def.Unload = Unload;
    def.Flags = EAddonFlags_None;
    def.Provider = EUpdateProvider_GitHub;
    def.UpdateLink = "https://github.com/Azrub/GolemHelper";
    return &def;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}