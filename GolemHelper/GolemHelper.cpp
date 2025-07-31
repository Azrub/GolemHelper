#include <Windows.h>
#include <string>
#include "Common/Globals.h"
#include "Utils/FileUtils.h"
#include "Utils/MapUtils.h"
#include "Config/ConfigManager.h"
#include "UI/UIManager.h"
#include "Input/KeybindManager.h"
#include "Dependencies/imgui/imgui.h"

void Load(AddonAPI* aApi) {
    g_api = aApi;

    ImGui::SetCurrentContext((ImGuiContext*)g_api->ImguiContext);
    ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))g_api->ImguiMalloc, (void(*)(void*, void*))g_api->ImguiFree);

    g_nexusLink = (NexusLinkData*)g_api->DataLink.Get("DL_NEXUS_LINK");

    g_mumbleData = (Mumble::Data*)g_api->DataLink.Get("DL_MUMBLE_LINK");

    g_state.enabled = true;

    ConfigManager::LoadCustomDelaySettings();
    FileUtils::CopyResourceIcons();

    g_api->Renderer.Register(ERenderType_Render, UIManager::RenderUI);
    g_api->Renderer.Register(ERenderType_OptionsRender, UIManager::RenderOptions);

    KeybindManager::RegisterKeybinds();

    g_api->Textures.GetOrCreateFromFile("GOLEM_HELPER_ICON", "addons/GolemHelper/icons/GOLEM_HELPER_ICON.png");
    g_api->Textures.GetOrCreateFromFile("GOLEM_HELPER_ICON_HOVER", "addons/GolemHelper/icons/GOLEM_HELPER_ICON_HOVER.png");

    MapUtils::UpdateQuickAccessVisibility();

    g_api->Log(ELogLevel_INFO, "GolemHelper", "=== GolemHelper v1.3.0.0 Loaded ===");
    g_api->Log(ELogLevel_INFO, "GolemHelper", "<c=#00ff00>GolemHelper addon</c> loaded successfully!");
}

void Unload() {
    if (g_api) {
        if (g_state.quickAccessVisible) {
            g_api->QuickAccess.Remove("GolemHelper.ToggleUI");
        }
        g_api->Renderer.Deregister(UIManager::RenderUI);
        g_api->Renderer.Deregister(UIManager::RenderOptions);
        KeybindManager::UnregisterKeybinds();
    }

    g_api->Log(ELogLevel_INFO, "GolemHelper", "<c=#ff0000>GolemHelper signing off</c>, it was an honor commander.");
    g_api = nullptr;
    g_mumbleData = nullptr;
    g_state.enabled = false;
    g_state.showUI = false;
    g_state.quickAccessVisible = false;
}

extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef() {
    static AddonDefinition def;
    def.Signature = -424248;
    def.APIVersion = NEXUS_API_VERSION;
    def.Name = "GolemHelper";
    def.Version = { 1, 3, 0, 0 };
    def.Author = "Azrub";
    def.Description = "Automates the process of setting optimal boon and golem configurations in the training area";
    def.Load = Load;
    def.Unload = Unload;
    def.Flags = EAddonFlags_None;
    def.Provider = EUpdateProvider_GitHub;
    def.UpdateLink = "https://github.com/Azrub/GolemHelper";
    return &def;
}