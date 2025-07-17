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
        configFile << "useCustomDelays=" << (g_state.useCustomDelays ? "1" : "0") << std::endl;
        configFile << "stepDelay=" << g_state.stepDelay << std::endl;

        configFile.close();

        char logBuffer[200];
        sprintf_s(logBuffer, "Custom delay settings saved: enabled=%s, delay=%dms",
            g_state.useCustomDelays ? "true" : "false", g_state.stepDelay);
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

            if (key == "useCustomDelays") {
                g_state.useCustomDelays = (value == "1");
            }
            else if (key == "stepDelay") {
                int delay = std::stoi(value);
                if (delay >= 100 && delay <= 1000) {
                    g_state.stepDelay = delay;
                }
            }
        }

        configFile.close();

        char logBuffer[200];
        sprintf_s(logBuffer, "Custom delay settings loaded: enabled=%s, delay=%dms",
            g_state.useCustomDelays ? "true" : "false", g_state.stepDelay);
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);

    }
    catch (...) {
        g_api->Log(ELogLevel_INFO, "GolemHelper", "Could not load config file, using defaults");
    }
}