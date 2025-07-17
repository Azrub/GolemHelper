#pragma once
#include "Dependencies/nexus/Nexus.h"

void Load(AddonAPI* aApi);
void Unload();
extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef();