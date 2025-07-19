#include <Windows.h>
#include <string>
#include "UIManager.h"
#include "../Common/Globals.h"
#include "../Config/ConfigManager.h"
#include "../Dependencies/imgui/imgui.h"

void UIManager::RenderUI() {
    if (!g_state.showUI) return;

    ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("GolemHelper", &g_state.showUI, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "GolemHelper v1.2.4.0");
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

        ImGui::Text(g_state.environmentDamage ? "Healer Modes:" : "DPS Modes:");

        if (g_state.environmentDamage) {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
            ImGui::RadioButton("Normal Mode", false);
            ImGui::PopStyleVar();
        }
        else {
            if (ImGui::RadioButton("Normal Mode", !g_state.isQuickDps && !g_state.isAlacDps)) {
                g_state.isQuickDps = false;
                g_state.isAlacDps = false;
            }
        }

        if (ImGui::RadioButton(g_state.environmentDamage ? "qHeal (Skip Quickness)" : "Quick DPS (Skip Quickness)", g_state.isQuickDps)) {
            g_state.isQuickDps = true;
            g_state.isAlacDps = false;
        }

        if (ImGui::RadioButton(g_state.environmentDamage ? "aHeal (Skip Alacrity)" : "Alac DPS (Skip Alacrity)", g_state.isAlacDps)) {
            g_state.isQuickDps = false;
            g_state.isAlacDps = true;
        }

        ImGui::Spacing();
        ImGui::Text("Healer Bench:");

        bool wasEnvironmentDamage = g_state.environmentDamage;
        ImGui::Checkbox("Environment Damage", &g_state.environmentDamage);

        if (g_state.environmentDamage && !wasEnvironmentDamage) {
            if (!g_state.isQuickDps && !g_state.isAlacDps) {
                g_state.isQuickDps = true;
            }
        }

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

        if (ImGui::RadioButton("Large", g_state.hitboxType == HITBOX_LARGE)) {
            g_state.hitboxType = HITBOX_LARGE;
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
            if (g_nexusLink && g_nexusLink->Width > 0 && g_nexusLink->Height > 0) {
                ImGui::Text("Resolution: %dx%d", g_nexusLink->Width, g_nexusLink->Height);
                ImGui::Text("UI Scale: %.2f", g_nexusLink->Scaling);
                float dpiScale = (float)g_nexusLink->Width / 1920.0f;
                ImGui::Text("DPI Scale: %.3f", dpiScale);
                ImGui::Text("Debug samples: %d", g_state.debugCounter);
            }
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Set Custom Delays")) {

            bool oldUseCustomDelays = g_state.useCustomDelays;
            int oldStepDelay = g_state.stepDelay;
            int oldInitialDelay = g_state.initialDelay;

            ImGui::Checkbox("Use Custom Delays", &g_state.useCustomDelays);

            ImGui::Spacing();

            ImGui::Text("Initial Delay (after F key):");
            ImGui::SetNextItemWidth(205);
            ImGui::SliderInt("##initial", &g_state.initialDelay, 100, 1000, "%d ms");

            ImGui::Text("Step Delay (between clicks):");
            ImGui::SetNextItemWidth(205);
            ImGui::SliderInt("##step", &g_state.stepDelay, 100, 1000, "%d ms");

            ImGui::Spacing();

            if (ImGui::Button("Reset to Default", ImVec2(120, 0))) {
                g_state.stepDelay = 290;
                g_state.initialDelay = 390;
                ConfigManager::SaveCustomDelaySettings();
            }
            ImGui::SameLine();
            if (ImGui::Button("Slow Mode", ImVec2(80, 0))) {
                g_state.stepDelay = 1000;
                g_state.initialDelay = 600;
                ConfigManager::SaveCustomDelaySettings();
            }

            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "Increase delays if clicks fail");

            if (oldUseCustomDelays != g_state.useCustomDelays ||
                oldStepDelay != g_state.stepDelay ||
                oldInitialDelay != g_state.initialDelay) {
                ConfigManager::SaveCustomDelaySettings();
            }
        }
    }

    ImGui::End();
}

void UIManager::RenderOptions() {
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
        g_state.showTimingSettings = false;
        g_state.stepDelay = 290;
        g_state.initialDelay = 390;
        g_state.boonsEnabled = true;
        g_state.golemEnabled = true;

        ConfigManager::SaveCustomDelaySettings();
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
        g_state.hitboxType == HITBOX_MEDIUM ? "Medium" : "Large";
    ImGui::Text("- Hitbox: %s", hitboxName);

    ImGui::Text("- Timing: %s", g_state.useCustomDelays ? "Custom" : "Default");

    if (g_state.useCustomDelays) {
        ImGui::Text("- Step Delay: %d ms", g_state.stepDelay);
        ImGui::Text("- Initial Delay: %d ms", g_state.initialDelay);
    }
}