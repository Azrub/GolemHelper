#pragma once

class UIManager {
public:
    static void RenderUI();
    static void RenderOptions();

private:
    static void RenderSettingsTab();
    static void RenderTemplatesTab();
};