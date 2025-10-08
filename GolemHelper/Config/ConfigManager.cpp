#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include "ConfigManager.h"
#include "../Common/Globals.h"
#include "../Utils/FileUtils.h"

void ConfigManager::SaveCustomDelaySettings() {
    if (!g_api) return;

    std::string configPath = FileUtils::GetConfigFilePath();

    try {
        std::ofstream configFile(configPath);
        if (!configFile.is_open()) {
            g_api->Log(ELogLevel_WARNING, "GolemHelper", "Could not create config file");
            return;
        }

        configFile << "[GolemHelper]" << std::endl;
        configFile << "initialDelay=" << g_state.initialDelay << std::endl;
        configFile << "stepDelay=" << g_state.stepDelay << std::endl;
        configFile << "alwaysHideIcon=" << (g_state.alwaysHideIcon ? "1" : "0") << std::endl;
        configFile << "autoShowHideUI=" << (g_state.autoShowHideUI ? "1" : "0") << std::endl;
        configFile << "alwaysLoadLastSettings=" << (g_state.alwaysLoadLastSettings ? "1" : "0") << std::endl;

        configFile.close();

        char logBuffer[512];
        sprintf_s(logBuffer, "Settings saved: initialDelay=%dms, stepDelay=%dms, alwaysHideIcon=%s, autoShowHideUI=%s, alwaysLoadLastSettings=%s",
            g_state.initialDelay, g_state.stepDelay,
            g_state.alwaysHideIcon ? "true" : "false",
            g_state.autoShowHideUI ? "true" : "false",
            g_state.alwaysLoadLastSettings ? "true" : "false");
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);

    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Failed to save config file");
    }
}

void ConfigManager::LoadCustomDelaySettings() {
    if (!g_api) return;

    std::string configPath = FileUtils::GetConfigFilePath();

    try {
        std::ifstream configFile(configPath);
        if (!configFile.is_open()) {
            g_api->Log(ELogLevel_INFO, "GolemHelper", "No config file found, using defaults");
            return;
        }

        std::string line;
        while (std::getline(configFile, line)) {
            if (line.empty() || line[0] == '[') continue;

            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) continue;

            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);

            if (key == "initialDelay") {
                int delay = std::stoi(value);
                if (delay >= 100 && delay <= 1000) {
                    g_state.initialDelay = delay;
                }
            }
            else if (key == "stepDelay") {
                int delay = std::stoi(value);
                if (delay >= 100 && delay <= 1000) {
                    g_state.stepDelay = delay;
                }
            }
            else if (key == "alwaysHideIcon") {
                g_state.alwaysHideIcon = (value == "1");
            }
            else if (key == "autoShowHideUI") {
                g_state.autoShowHideUI = (value == "1");
            }
            else if (key == "alwaysLoadLastSettings") {
                g_state.alwaysLoadLastSettings = (value == "1");
            }
        }

        configFile.close();

        char logBuffer[512];
        sprintf_s(logBuffer, "Settings loaded: initialDelay=%dms, stepDelay=%dms, alwaysHideIcon=%s, autoShowHideUI=%s, alwaysLoadLastSettings=%s",
            g_state.initialDelay, g_state.stepDelay,
            g_state.alwaysHideIcon ? "true" : "false",
            g_state.autoShowHideUI ? "true" : "false",
            g_state.alwaysLoadLastSettings ? "true" : "false");
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);

    }
    catch (...) {
        g_api->Log(ELogLevel_INFO, "GolemHelper", "Could not load config file, using defaults");
    }
}

void ConfigManager::SaveLastUsedSettings() {
    if (!g_api) return;

    std::string addonPath = g_api->Paths.GetAddonDirectory("GolemHelper");
    std::string settingsPath = addonPath + "\\last_settings.ini";

    try {
        std::ofstream settingsFile(settingsPath);
        if (!settingsFile.is_open()) {
            g_api->Log(ELogLevel_WARNING, "GolemHelper", "Could not create last settings file");
            return;
        }

        settingsFile << "[LastUsedSettings]" << std::endl;
        settingsFile << "isQuickDps=" << (g_state.isQuickDps ? "1" : "0") << std::endl;
        settingsFile << "isAlacDps=" << (g_state.isAlacDps ? "1" : "0") << std::endl;
        settingsFile << "showBoonAdvanced=" << (g_state.showBoonAdvanced ? "1" : "0") << std::endl;
        settingsFile << "environmentDamage=" << (g_state.environmentDamage ? "1" : "0") << std::endl;
        settingsFile << "envDamageLevel=" << static_cast<int>(g_state.envDamageLevel) << std::endl;
        settingsFile << "skipBurning=" << (g_state.skipBurning ? "1" : "0") << std::endl;
        settingsFile << "showAdvanced=" << (g_state.showAdvanced ? "1" : "0") << std::endl;
        settingsFile << "skipConfusion=" << (g_state.skipConfusion ? "1" : "0") << std::endl;
        settingsFile << "skipSlow=" << (g_state.skipSlow ? "1" : "0") << std::endl;
        settingsFile << "addImmobilize=" << (g_state.addImmobilize ? "1" : "0") << std::endl;
        settingsFile << "addBlind=" << (g_state.addBlind ? "1" : "0") << std::endl;
        settingsFile << "fiveBleedingStacks=" << (g_state.fiveBleedingStacks ? "1" : "0") << std::endl;
        settingsFile << "hitboxType=" << static_cast<int>(g_state.hitboxType) << std::endl;
        settingsFile << "addResistance=" << (g_state.addResistance ? "1" : "0") << std::endl;
        settingsFile << "addStability=" << (g_state.addStability ? "1" : "0") << std::endl;
        settingsFile << "skipAegis=" << (g_state.skipAegis ? "1" : "0") << std::endl;

        settingsFile.close();
        g_api->Log(ELogLevel_INFO, "GolemHelper", "Last used settings saved");

    }
    catch (...) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Failed to save last settings file");
    }
}

void ConfigManager::LoadLastUsedSettings() {
    if (!g_api) return;

    std::string addonPath = g_api->Paths.GetAddonDirectory("GolemHelper");
    std::string settingsPath = addonPath + "\\last_settings.ini";

    try {
        std::ifstream settingsFile(settingsPath);
        if (!settingsFile.is_open()) {
            g_api->Log(ELogLevel_INFO, "GolemHelper", "No last settings file found");
            return;
        }

        std::string line;
        while (std::getline(settingsFile, line)) {
            if (line.empty() || line[0] == '[') continue;

            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) continue;

            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);

            if (key == "isQuickDps") g_state.isQuickDps = (value == "1");
            else if (key == "isAlacDps") g_state.isAlacDps = (value == "1");
            else if (key == "showBoonAdvanced") g_state.showBoonAdvanced = (value == "1");
            else if (key == "environmentDamage") g_state.environmentDamage = (value == "1");
            else if (key == "envDamageLevel") g_state.envDamageLevel = static_cast<EnvironmentDamageLevel>(std::stoi(value));
            else if (key == "showAdvanced") g_state.showAdvanced = (value == "1");
            else if (key == "skipBurning") g_state.skipBurning = (value == "1");
            else if (key == "skipConfusion") g_state.skipConfusion = (value == "1");
            else if (key == "skipSlow") g_state.skipSlow = (value == "1");
            else if (key == "addImmobilize") g_state.addImmobilize = (value == "1");
            else if (key == "addBlind") g_state.addBlind = (value == "1");
            else if (key == "fiveBleedingStacks") g_state.fiveBleedingStacks = (value == "1");
            else if (key == "hitboxType") g_state.hitboxType = static_cast<HitboxType>(std::stoi(value));
            else if (key == "addResistance") g_state.addResistance = (value == "1");
            else if (key == "addStability") g_state.addStability = (value == "1");
            else if (key == "skipAegis") g_state.skipAegis = (value == "1");
        }

        settingsFile.close();
        g_api->Log(ELogLevel_INFO, "GolemHelper", "Last used settings loaded");

    }
    catch (...) {
        g_api->Log(ELogLevel_INFO, "GolemHelper", "Could not load last settings file");
    }
}