#pragma once

class AutomationLogic {
public:
    static bool ShouldSkipBoonStep(int stepIndex);
    static bool ShouldSkipGolemStep(int stepIndex);
    static void ApplyBoons();
    static void ApplyHealerBoons();
    static void SpawnGolem();
    static void RespawnGolem();
    static void RemoveAndRespawnGolem();
};