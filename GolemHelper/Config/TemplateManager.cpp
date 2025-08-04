#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "TemplateManager.h"
#include "../Common/Globals.h"
#include "../Utils/FileUtils.h"

void TemplateManager::LoadTemplates() {
    if (!g_api) return;

    CreateDefaultTemplates();

    std::string templatePath = FileUtils::GetAddonPath() + "\\templates.cfg";

    try {
        std::ifstream file(templatePath);
        if (!file.is_open()) {
            g_api->Log(ELogLevel_INFO, "GolemHelper", "No user templates file found");
            SaveTemplates();
            return;
        }

        std::string line;
        GolemTemplate currentTemplate;
        bool inTemplate = false;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            if (line.find("[Template]") == 0) {
                if (inTemplate && !currentTemplate.name.empty()) {
                    g_state.templates.push_back(currentTemplate);
                }
                currentTemplate = GolemTemplate();
                inTemplate = true;
                continue;
            }

            if (!inTemplate) continue;

            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) continue;

            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);

            if (key == "name") {
                currentTemplate.name = value;
            }
            else if (key == "isQuickDps") {
                currentTemplate.isQuickDps = (value == "1");
            }
            else if (key == "isAlacDps") {
                currentTemplate.isAlacDps = (value == "1");
            }
            else if (key == "environmentDamage") {
                currentTemplate.environmentDamage = (value == "1");
            }
            else if (key == "envDamageLevel") {
                currentTemplate.envDamageLevel = (EnvironmentDamageLevel)std::stoi(value);
            }
            else if (key == "skipSlow") {
                currentTemplate.skipSlow = (value == "1");
            }
            else if (key == "skipBurning") {
                currentTemplate.skipBurning = (value == "1");
            }
            else if (key == "fiveBleedingStacks") {
                currentTemplate.fiveBleedingStacks = (value == "1");
            }
            else if (key == "hitboxType") {
                currentTemplate.hitboxType = (HitboxType)std::stoi(value);
            }
            else if (key == "addResistance") {
                currentTemplate.addResistance = (value == "1");
            }
            else if (key == "addStability") {
                currentTemplate.addStability = (value == "1");
            }
            else if (key == "skipAegis") {
                currentTemplate.skipAegis = (value == "1");
            }
        }

        if (inTemplate && !currentTemplate.name.empty()) {
            g_state.templates.push_back(currentTemplate);
        }

        file.close();

        int userTemplateCount = 0;
        for (const auto& temp : g_state.templates) {
            if (!temp.isDefaultTemplate) userTemplateCount++;
        }

        char logBuffer[150];
        sprintf_s(logBuffer, "Loaded %d user templates", userTemplateCount);
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);

    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Failed to load user templates");
    }
}

void TemplateManager::SaveTemplates() {
    if (!g_api) return;

    std::string templatePath = FileUtils::GetAddonPath() + "\\templates.cfg";

    try {
        std::ofstream file(templatePath);
        if (!file.is_open()) {
            g_api->Log(ELogLevel_WARNING, "GolemHelper", "Could not create templates file");
            return;
        }

        file << "# GolemHelper User Templates Configuration\n";

        int savedCount = 0;
        for (const auto& temp : g_state.templates) {
            if (!temp.isDefaultTemplate) {
                file << "[Template]\n";
                file << "name=" << temp.name << "\n";
                file << "isQuickDps=" << (temp.isQuickDps ? "1" : "0") << "\n";
                file << "isAlacDps=" << (temp.isAlacDps ? "1" : "0") << "\n";
                file << "environmentDamage=" << (temp.environmentDamage ? "1" : "0") << "\n";
                file << "envDamageLevel=" << temp.envDamageLevel << "\n";
                file << "skipSlow=" << (temp.skipSlow ? "1" : "0") << "\n";
                file << "skipBurning=" << (temp.skipBurning ? "1" : "0") << "\n";
                file << "fiveBleedingStacks=" << (temp.fiveBleedingStacks ? "1" : "0") << "\n";
                file << "hitboxType=" << temp.hitboxType << "\n";
                file << "addResistance=" << (temp.addResistance ? "1" : "0") << "\n";
                file << "addStability=" << (temp.addStability ? "1" : "0") << "\n";
                file << "skipAegis=" << (temp.skipAegis ? "1" : "0") << "\n\n";
                savedCount++;
            }
        }

        file.close();

        char logBuffer[150];
        sprintf_s(logBuffer, "Saved %d user templates", savedCount);
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);

    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Failed to save templates");
    }
}

void TemplateManager::SaveCurrentAsTemplate(const std::string& name) {
    GolemTemplate newTemplate = CreateTemplateFromCurrentSettings();
    newTemplate.name = name;

    g_state.templates.push_back(newTemplate);
    SaveTemplates();

    if (g_api) {
        char logBuffer[200];
        sprintf_s(logBuffer, "Template '%s' saved successfully", name.c_str());
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);
    }
}

void TemplateManager::LoadTemplate(int index) {
    if (index < 0 || index >= g_state.templates.size()) return;

    ApplyTemplateToSettings(g_state.templates[index]);

    if (!g_state.templates[index].isDefaultTemplate) {
        g_state.selectedTemplateIndex = index;
        g_state.lastUserTemplateIndex = index;
    }

    if (g_api) {
        char logBuffer[200];
        sprintf_s(logBuffer, "Template '%s' loaded", g_state.templates[index].name.c_str());
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);
    }
}

void TemplateManager::DeleteTemplate(int index) {
    if (index < 0 || index >= g_state.templates.size()) return;

    std::string templateName = g_state.templates[index].name;
    g_state.templates.erase(g_state.templates.begin() + index);

    if (g_state.selectedTemplateIndex == index) {
        g_state.selectedTemplateIndex = -1;
    }
    else if (g_state.selectedTemplateIndex > index) {
        g_state.selectedTemplateIndex--;
    }

    SaveTemplates();

    if (g_api) {
        char logBuffer[200];
        sprintf_s(logBuffer, "Template '%s' deleted", templateName.c_str());
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);
    }
}

void TemplateManager::RenameTemplate(int index, const std::string& newName) {
    if (index < 0 || index >= g_state.templates.size()) return;

    std::string oldName = g_state.templates[index].name;
    g_state.templates[index].name = newName;
    SaveTemplates();

    if (g_api) {
        char logBuffer[300];
        sprintf_s(logBuffer, "Template renamed from '%s' to '%s'", oldName.c_str(), newName.c_str());
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);
    }
}

GolemTemplate TemplateManager::CreateTemplateFromCurrentSettings() {
    GolemTemplate temp;
    temp.isQuickDps = g_state.isQuickDps;
    temp.isAlacDps = g_state.isAlacDps;
    temp.environmentDamage = g_state.environmentDamage;
    temp.envDamageLevel = g_state.envDamageLevel;
    temp.skipSlow = g_state.skipSlow;
    temp.skipBurning = g_state.skipBurning;
    temp.fiveBleedingStacks = g_state.fiveBleedingStacks;
    temp.hitboxType = g_state.hitboxType;
    temp.addResistance = g_state.addResistance;
    temp.addStability = g_state.addStability;
    temp.skipAegis = g_state.skipAegis;
    temp.isDefaultTemplate = false;
    return temp;
}

void TemplateManager::ApplyTemplateToSettings(const GolemTemplate& temp) {
    g_state.isQuickDps = temp.isQuickDps;
    g_state.isAlacDps = temp.isAlacDps;
    g_state.environmentDamage = temp.environmentDamage;
    g_state.envDamageLevel = temp.envDamageLevel;
    g_state.skipSlow = temp.skipSlow;
    g_state.skipBurning = temp.skipBurning;
    g_state.fiveBleedingStacks = temp.fiveBleedingStacks;
    g_state.hitboxType = temp.hitboxType;
    g_state.addResistance = temp.addResistance;
    g_state.addStability = temp.addStability;
    g_state.skipAegis = temp.skipAegis;

    if (temp.isDefaultTemplate) {
        g_state.showAdvanced = false;
        g_state.showBoonAdvanced = false;
    }
    else {
        if (temp.skipSlow || temp.skipBurning || temp.fiveBleedingStacks) {
            g_state.showAdvanced = true;
        }
        if (temp.addResistance || temp.addStability || temp.skipAegis) {
            g_state.showBoonAdvanced = true;
        }
    }
}

void TemplateManager::CreateDefaultTemplates() {
    g_state.templates.clear();

    GolemTemplate dps;
    dps.name = "DPS";
    dps.isDefaultTemplate = true;
    g_state.templates.push_back(dps);

    GolemTemplate quickDps;
    quickDps.name = "Quick DPS";
    quickDps.isQuickDps = true;
    quickDps.isDefaultTemplate = true;
    g_state.templates.push_back(quickDps);

    GolemTemplate alacDps;
    alacDps.name = "Alac DPS";
    alacDps.isAlacDps = true;
    alacDps.isDefaultTemplate = true;
    g_state.templates.push_back(alacDps);

    GolemTemplate qHeal;
    qHeal.name = "qHeal";
    qHeal.isQuickDps = true;
    qHeal.environmentDamage = true;
    qHeal.envDamageLevel = ENV_MILD;
    qHeal.isDefaultTemplate = true;
    g_state.templates.push_back(qHeal);

    GolemTemplate aHeal;
    aHeal.name = "aHeal";
    aHeal.isAlacDps = true;
    aHeal.environmentDamage = true;
    aHeal.envDamageLevel = ENV_MILD;
    aHeal.isDefaultTemplate = true;
    g_state.templates.push_back(aHeal);
}