#pragma once
#include <string>
#include <Windows.h>

class FileUtils {
public:
    static std::string GetAddonPath();
    static std::string GetConfigFilePath();
    static bool ExtractResourceToFile(HMODULE hModule, int resourceID, const std::string& filePath);
    static void CopyResourceIcons();
};