#pragma once

class AutomationLogic {
public:
    static bool ShouldSkipBoonStep(int stepIndex);
    static bool ShouldSkipGolemStep(int stepIndex);
    static void ApplyAllBoons();
    static void ApplyHealerBoons();
    static void ApplyGolemSettings();
};