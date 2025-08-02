#pragma once
#include "../Common/Types.h"

class TemplateManager {
public:
    static void LoadTemplates();
    static void SaveTemplates();
    static void SaveCurrentAsTemplate(const std::string& name);
    static void LoadTemplate(int index);
    static void DeleteTemplate(int index);
    static void RenameTemplate(int index, const std::string& newName);
    static GolemTemplate CreateTemplateFromCurrentSettings();
    static void ApplyTemplateToSettings(const GolemTemplate& temp);
    static void CreateDefaultTemplates();
};