#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include "FileUtils.h"
#include "../Common/Globals.h"
#include "../resource.h"

std::string FileUtils::GetAddonPath() {
    char gameDir[MAX_PATH];
    GetModuleFileNameA(NULL, gameDir, MAX_PATH);

    std::string gamePath = gameDir;
    size_t lastSlash = gamePath.find_last_of("\\");
    if (lastSlash != std::string::npos) {
        gamePath = gamePath.substr(0, lastSlash);
    }

    std::string addonPath = gamePath + "\\addons\\GolemHelper";
    CreateDirectoryA(addonPath.c_str(), NULL);

    return addonPath;
}

std::string FileUtils::GetConfigFilePath() {
    std::string configPath = GetAddonPath() + "\\config.ini";

    if (g_api) {
        char logBuffer[500];
        sprintf_s(logBuffer, "Config file path: %s", configPath.c_str());
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);
    }

    return configPath;
}

bool FileUtils::ExtractResourceToFile(HMODULE hModule, int resourceID, const std::string& filePath) {
    HRSRC hRes = FindResourceA(hModule, MAKEINTRESOURCEA(resourceID), "PNG");
    if (!hRes) return false;

    HGLOBAL hData = LoadResource(hModule, hRes);
    if (!hData) return false;

    LPVOID pData = LockResource(hData);
    if (!pData) return false;

    DWORD dataSize = SizeofResource(hModule, hRes);
    if (!dataSize) return false;

    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) return false;

    file.write(static_cast<const char*>(pData), dataSize);
    file.close();

    return true;
}

void FileUtils::CopyResourceIcons() {
    if (!g_api) return;

    std::string addonPath = GetAddonPath();
    std::string iconsPath = addonPath + "\\icons";
    CreateDirectoryA(iconsPath.c_str(), NULL);

    std::string iconDestPath = iconsPath + "\\GOLEM_HELPER_ICON.png";
    std::string iconHoverDestPath = iconsPath + "\\GOLEM_HELPER_ICON_HOVER.png";

    if (GetFileAttributesA(iconDestPath.c_str()) != INVALID_FILE_ATTRIBUTES &&
        GetFileAttributesA(iconHoverDestPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
        g_api->Log(ELogLevel_INFO, "GolemHelper", "Icons already exist, skipping unpack");
        return;
    }

    HMODULE hModule = GetModuleHandleA("GolemHelper.dll");
    if (!hModule) {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Failed to get module handle");
        return;
    }

    if (ExtractResourceToFile(hModule, IDB_PNG1, iconDestPath)) {
        char logBuffer[300];
        sprintf_s(logBuffer, "Extracted icon from resources: %s", iconDestPath.c_str());
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);
    }
    else {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Failed to extract normal icon from resources");
    }

    if (ExtractResourceToFile(hModule, IDB_PNG2, iconHoverDestPath)) {
        char logBuffer[300];
        sprintf_s(logBuffer, "Extracted hover icon from resources: %s", iconHoverDestPath.c_str());
        g_api->Log(ELogLevel_INFO, "GolemHelper", logBuffer);
    }
    else {
        g_api->Log(ELogLevel_WARNING, "GolemHelper", "Failed to extract hover icon from resources");
    }
}