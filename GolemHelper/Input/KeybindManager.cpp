#include <Windows.h>
#include <string>
#include "KeybindManager.h"
#include "../Common/Globals.h"
#include "../Automation/AutomationLogic.h"
#include "../Automation/CoordinateUtils.h"

void KeybindManager::RegisterKeybinds() {
    if (!g_api) return;

    Keybind kb_empty = { 0, false, false, false };
    g_api->InputBinds.RegisterWithStruct("GolemHelper.ApplyBoons", HandleBoonKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.SpawnGolem", HandleGolemKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.RespawnGolem", HandleRespawnGolemKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.QuickDPS", HandleQuickDpsKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.AlacDPS", HandleAlacDpsKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.Toggle", HandleToggleKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.ToggleUI", HandleUIToggleKeybind, kb_empty);
    g_api->InputBinds.RegisterWithStruct("GolemHelper.DebugMouse", HandleDebugKeybind, kb_empty);
}

void KeybindManager::UnregisterKeybinds() {
    if (!g_api) return;

    g_api->InputBinds.Deregister("GolemHelper.ApplyBoons");
    g_api->InputBinds.Deregister("GolemHelper.SpawnGolem");
    g_api->InputBinds.Deregister("GolemHelper.RespawnGolem");
    g_api->InputBinds.Deregister("GolemHelper.QuickDPS");
    g_api->InputBinds.Deregister("GolemHelper.AlacDPS");
    g_api->InputBinds.Deregister("GolemHelper.Toggle");
    g_api->InputBinds.Deregister("GolemHelper.ToggleUI");
    g_api->InputBinds.Deregister("GolemHelper.DebugMouse");
}

void KeybindManager::HandleBoonKeybind(const char* id, bool release) {
    if (!release && g_state.enabled) {
        AutomationLogic::ApplyBoons();
    }
}

void KeybindManager::HandleGolemKeybind(const char* id, bool release) {
    if (!release && g_state.enabled) {
        AutomationLogic::SpawnGolem();
    }
}

void KeybindManager::HandleRespawnGolemKeybind(const char* id, bool release) {
    if (!release && g_state.enabled) {
        AutomationLogic::RespawnGolem();
    }
}

void KeybindManager::HandleToggleKeybind(const char* id, bool release) {
    if (!release) {
        g_state.enabled = !g_state.enabled;
        g_api->UI.SendAlert(g_state.enabled ? "GolemHelper enabled" : "GolemHelper disabled");
    }
}

void KeybindManager::HandleUIToggleKeybind(const char* id, bool release) {
    if (!release) {
        g_state.showUI = !g_state.showUI;
    }
}

void KeybindManager::HandleDebugKeybind(const char* id, bool release) {
    if (!release) {
        CoordinateUtils::DebugMousePosition();
    }
}

void KeybindManager::HandleQuickDpsKeybind(const char* id, bool release) {
    if (!release) {
        g_state.isQuickDps = !g_state.isQuickDps;
        if (g_state.isQuickDps) {
            g_state.isAlacDps = false;
        }
        g_api->UI.SendAlert(g_state.isQuickDps ? "Quick DPS mode enabled" : "Quick DPS mode disabled");
    }
}

void KeybindManager::HandleAlacDpsKeybind(const char* id, bool release) {
    if (!release) {
        g_state.isAlacDps = !g_state.isAlacDps;
        if (g_state.isAlacDps) {
            g_state.isQuickDps = false;
        }
        g_api->UI.SendAlert(g_state.isAlacDps ? "Alac DPS mode enabled" : "Alac DPS mode disabled");
    }
}