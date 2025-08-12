#pragma once

class KeybindManager {
public:
    static void RegisterKeybinds();
    static void UnregisterKeybinds();

    static void HandleBoonKeybind(const char* id, bool release);
    static void HandleGolemKeybind(const char* id, bool release);
    static void HandleRespawnGolemKeybind(const char* id, bool release);
    static void HandleQuickDpsKeybind(const char* id, bool release);
    static void HandleAlacDpsKeybind(const char* id, bool release);
    static void HandleToggleKeybind(const char* id, bool release);
    static void HandleUIToggleKeybind(const char* id, bool release);
    static void HandleDebugKeybind(const char* id, bool release);
};