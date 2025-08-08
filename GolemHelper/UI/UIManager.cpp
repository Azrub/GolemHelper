#include <Windows.h>
#include <string>
#include "UIManager.h"
#include "../Common/Globals.h"
#include "../Config/ConfigManager.h"
#include "../Utils/MapUtils.h"
#include "../Config/TemplateManager.h"
#include "../Dependencies/imgui/imgui.h"

void UIManager::RenderUI() {
    MapUtils::UpdateQuickAccessVisibility();

    if (!g_state.showUI) return;

    ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("GolemHelper", &g_state.showUI, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "GolemHelper v1.5.1.0");
        ImGui::Separator();

        if (ImGui::BeginTabBar("GolemHelperTabs", ImGuiTabBarFlags_None)) {

            if (ImGui::BeginTabItem("Settings")) {
                RenderSettingsTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Templates")) {
                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130);

                std::string currentTemplateName = "None";
                bool foundMatchingTemplate = false;

                for (const auto& temp : g_state.templates) {
                    if (temp.isQuickDps == g_state.isQuickDps &&
                        temp.isAlacDps == g_state.isAlacDps &&
                        temp.environmentDamage == g_state.environmentDamage &&
                        temp.envDamageLevel == g_state.envDamageLevel &&
                        temp.skipBurning == g_state.skipBurning &&
                        temp.skipConfusion == g_state.skipConfusion &&
                        temp.skipSlow == g_state.skipSlow &&
                        temp.addImmobilize == g_state.addImmobilize &&
                        temp.addBlind == g_state.addBlind &&
                        temp.fiveBleedingStacks == g_state.fiveBleedingStacks &&
                        temp.hitboxType == g_state.hitboxType &&
                        temp.addResistance == g_state.addResistance &&
                        temp.addStability == g_state.addStability &&
                        temp.skipAegis == g_state.skipAegis) {
                        currentTemplateName = temp.name;
                        foundMatchingTemplate = true;
                        break;
                    }
                }

                ImGui::Text("Current: ");
                ImGui::SameLine();
                if (foundMatchingTemplate) {
                    std::string truncatedName = currentTemplateName;
                    if (truncatedName.length() > 8) {
                        truncatedName = truncatedName.substr(0, 6) + "..";
                    }
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", truncatedName.c_str());
                }
                else {
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "None");
                }

                RenderTemplatesTab();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}

void UIManager::RenderSettingsTab() {
    ImGui::Text("Boon Configuration");

    if (ImGui::Button("Apply Boons", ImVec2(150, 0))) {
        if (g_state.enabled && MapUtils::IsInTrainingArea()) {
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
    ImGui::Text("Advanced:");

    if (g_state.environmentDamage) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
        ImGui::Checkbox("Boon Settings", &g_state.showBoonAdvanced);
        g_state.showBoonAdvanced = false;
        ImGui::PopStyleVar();
    }
    else {
        ImGui::Checkbox("Boon Settings", &g_state.showBoonAdvanced);
    }

    if (g_state.showBoonAdvanced && !g_state.environmentDamage) {
        ImGui::Checkbox("Add Resistance", &g_state.addResistance);
        ImGui::Checkbox("Add Stability", &g_state.addStability);
        ImGui::Checkbox("Skip Aegis", &g_state.skipAegis);
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

    if (ImGui::Button("Spawn Golem", ImVec2(150, 0))) {
        if (g_state.enabled && MapUtils::IsInTrainingArea()) {
            g_api->InputBinds.Invoke("GolemHelper.SpawnGolem", false);
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
        ImGui::Checkbox("Skip Burning", &g_state.skipBurning);
        ImGui::Checkbox("Skip Confusion", &g_state.skipConfusion);
        ImGui::Checkbox("Skip Slow", &g_state.skipSlow);
        ImGui::Checkbox("Add Immobilize", &g_state.addImmobilize);
        ImGui::Checkbox("Add Blind", &g_state.addBlind);
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

        ImGui::Spacing();
        ImGui::Text("Map Info:");
        unsigned int currentMapID = MapUtils::GetCurrentMapID();
        ImGui::Text("Current MapID: %u", currentMapID);
        ImGui::Text("Is Training Area: %s", MapUtils::IsInTrainingArea() ? "YES" : "NO");
        ImGui::Text("QuickAccess Visible: %s", g_state.quickAccessVisible ? "YES" : "NO");
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.5f, 0.7f, 0.8f));
    if (ImGui::CollapsingHeader("Set Custom Delays")) {

        int oldStepDelay = g_state.stepDelay;
        int oldInitialDelay = g_state.initialDelay;

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

        if (oldStepDelay != g_state.stepDelay || oldInitialDelay != g_state.initialDelay) {
            ConfigManager::SaveCustomDelaySettings();
        }
    }
    ImGui::PopStyleColor(2);
}

void UIManager::RenderTemplatesTab() {
    if (ImGui::Button("Apply Boons", ImVec2(120, 30))) {
        if (g_state.enabled && MapUtils::IsInTrainingArea()) {
            g_api->InputBinds.Invoke("GolemHelper.ApplyBoons", false);
        }
    }
    ImGui::SameLine();

    if (ImGui::Button("Spawn Golem", ImVec2(120, 30))) {
        if (g_state.enabled && MapUtils::IsInTrainingArea()) {
            g_api->InputBinds.Invoke("GolemHelper.SpawnGolem", false);
        }
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Template Management");
    ImGui::Separator();

    ImGui::Text("Save Current Settings:");
    ImGui::SetNextItemWidth(170);
    ImGui::InputText("##templateName", g_state.newTemplateName, sizeof(g_state.newTemplateName));
    ImGui::SameLine();

    if (ImGui::Button("Save", ImVec2(50, 0))) {
        if (strlen(g_state.newTemplateName) > 0) {
            TemplateManager::SaveCurrentAsTemplate(std::string(g_state.newTemplateName));
            memset(g_state.newTemplateName, 0, sizeof(g_state.newTemplateName));
        }
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Available Templates:");

    std::vector<int> userTemplateIndices;
    std::vector<const char*> userTemplateNames;

    for (int i = 0; i < g_state.templates.size(); i++) {
        if (!g_state.templates[i].isDefaultTemplate) {
            userTemplateIndices.push_back(i);
            userTemplateNames.push_back(g_state.templates[i].name.c_str());
        }
    }

    if (userTemplateNames.empty()) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No user templates saved");
    }
    else {
        int currentUserIndex = -1;
        if (g_state.lastUserTemplateIndex >= 0) {
            for (int i = 0; i < userTemplateIndices.size(); i++) {
                if (userTemplateIndices[i] == g_state.lastUserTemplateIndex) {
                    currentUserIndex = i;
                    break;
                }
            }
        }

        ImGui::SetNextItemWidth(170);
        if (ImGui::Combo("##templateList", &currentUserIndex, userTemplateNames.data(), userTemplateNames.size())) {
            g_state.selectedTemplateIndex = userTemplateIndices[currentUserIndex];
            g_state.lastUserTemplateIndex = userTemplateIndices[currentUserIndex];
        }

        ImGui::SameLine();
        if (ImGui::Button("Load", ImVec2(50, 0))) {
            if (currentUserIndex >= 0 && currentUserIndex < userTemplateIndices.size()) {
                TemplateManager::LoadTemplate(userTemplateIndices[currentUserIndex]);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Del", ImVec2(50, 0))) {
            if (currentUserIndex >= 0 && currentUserIndex < userTemplateIndices.size()) {
                TemplateManager::DeleteTemplate(userTemplateIndices[currentUserIndex]);
                g_state.selectedTemplateIndex = -1;
            }
        }

        if (g_state.lastUserTemplateIndex >= 0 &&
            g_state.lastUserTemplateIndex < g_state.templates.size()) {

            const auto& selectedTemplate = g_state.templates[g_state.lastUserTemplateIndex];

            ImGui::Spacing();
            ImGui::Separator();

            std::string displayName = selectedTemplate.name;
            if (displayName.length() > 20) {
                displayName = displayName.substr(0, 17) + "...";
            }
            ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", displayName.c_str());

            std::string modeText = "Normal";
            if (selectedTemplate.isQuickDps) modeText = "Quick DPS";
            else if (selectedTemplate.isAlacDps) modeText = "Alac DPS";

            if (selectedTemplate.environmentDamage) {
                modeText += " + Env ";
                switch (selectedTemplate.envDamageLevel) {
                case ENV_MILD: modeText += "Mild"; break;
                case ENV_MODERATE: modeText += "Moderate"; break;
                case ENV_EXTREME: modeText += "Extreme"; break;
                }
            }

            ImGui::Text("Mode: %s", modeText.c_str());

            const char* hitboxName = selectedTemplate.hitboxType == HITBOX_SMALL ? "Small" :
                selectedTemplate.hitboxType == HITBOX_MEDIUM ? "Medium" : "Large";
            ImGui::Text("Hitbox: %s", hitboxName);

            if (selectedTemplate.skipBurning || selectedTemplate.skipConfusion || selectedTemplate.skipSlow ||
                selectedTemplate.addImmobilize || selectedTemplate.addBlind || selectedTemplate.fiveBleedingStacks) {

                ImGui::Text("Conditions:");
                ImGui::Indent(15.0f);

                std::vector<std::string> conditions;
                if (selectedTemplate.skipBurning) conditions.push_back("Skip Burning");
                if (selectedTemplate.skipConfusion) conditions.push_back("Skip Confusion");
                if (selectedTemplate.skipSlow) conditions.push_back("Skip Slow");
                if (selectedTemplate.addImmobilize) conditions.push_back("Add Immobilize");
                if (selectedTemplate.addBlind) conditions.push_back("Add Blind");
                if (selectedTemplate.fiveBleedingStacks) conditions.push_back("5 Bleeding");

                std::string conditionsText = "";
                for (int i = 0; i < conditions.size(); i++) {
                    if (i > 0 && i % 2 != 0) conditionsText += ", ";
                    conditionsText += conditions[i];

                    if ((i + 1) % 2 == 0 || i == conditions.size() - 1) {
                        ImGui::TextUnformatted(conditionsText.c_str());
                        conditionsText = "";
                    }
                }

                ImGui::Unindent(15.0f);
            }

            if (selectedTemplate.addResistance || selectedTemplate.addStability || selectedTemplate.skipAegis) {
                ImGui::Text("Boon Settings:");
                ImGui::Indent(15.0f);

                std::vector<std::string> boonSettings;
                if (selectedTemplate.addResistance) boonSettings.push_back("Add Resistance");
                if (selectedTemplate.addStability) boonSettings.push_back("Add Stability");
                if (selectedTemplate.skipAegis) boonSettings.push_back("Skip Aegis");

                std::string boonText = "";
                for (int i = 0; i < boonSettings.size(); i++) {
                    if (i > 0 && i % 2 != 0) boonText += ", ";
                    boonText += boonSettings[i];

                    if ((i + 1) % 2 == 0 || i == boonSettings.size() - 1) {
                        ImGui::TextUnformatted(boonText.c_str());
                        boonText = "";
                    }
                }

                ImGui::Unindent(15.0f);
            }
        }
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Quick Load:");

    std::vector<std::string> defaultNames = { "DPS", "Quick DPS", "Alac DPS", "qHeal", "aHeal" };

    for (int i = 0; i < 3; i++) {
        const std::string& name = defaultNames[i];
        int templateIndex = -1;
        for (int j = 0; j < g_state.templates.size(); j++) {
            if (g_state.templates[j].name == name && g_state.templates[j].isDefaultTemplate) {
                templateIndex = j;
                break;
            }
        }

        if (templateIndex >= 0) {
            if (ImGui::Button(name.c_str(), ImVec2(80, 0))) {
                TemplateManager::LoadTemplate(templateIndex);
                g_state.selectedTemplateIndex = -1;
            }
            if (i < 2) ImGui::SameLine();
        }
    }

    for (int i = 3; i < 5; i++) {
        const std::string& name = defaultNames[i];
        int templateIndex = -1;
        for (int j = 0; j < g_state.templates.size(); j++) {
            if (g_state.templates[j].name == name && g_state.templates[j].isDefaultTemplate) {
                templateIndex = j;
                break;
            }
        }

        if (templateIndex >= 0) {
            if (ImGui::Button(name.c_str(), ImVec2(80, 0))) {
                TemplateManager::LoadTemplate(templateIndex);
                g_state.selectedTemplateIndex = -1;
            }
            if (i == 3) ImGui::SameLine();
        }
    }
}

void UIManager::RenderOptions() {
    ImGui::Separator();
    ImGui::Text("GolemHelper Settings");

    bool oldAlwaysHideIcon = g_state.alwaysHideIcon;
    ImGui::Checkbox("Always Hide Icon", &g_state.alwaysHideIcon);

    if (oldAlwaysHideIcon != g_state.alwaysHideIcon) {
        ConfigManager::SaveCustomDelaySettings();
        MapUtils::UpdateQuickAccessVisibility();
    }

    bool oldAutoShowHideUI = g_state.autoShowHideUI;
    ImGui::Checkbox("Auto Show/Hide UI", &g_state.autoShowHideUI);

    if (oldAutoShowHideUI != g_state.autoShowHideUI) {
        ConfigManager::SaveCustomDelaySettings();
    }

    ImGui::Checkbox("Enable debug mode", &g_state.debugMode);
}