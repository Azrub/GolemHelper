#pragma once

class ConfigManager {
public:
    static void SaveCustomDelaySettings();
    static void LoadCustomDelaySettings();
    static void SaveLastUsedSettings();
    static void LoadLastUsedSettings();
};