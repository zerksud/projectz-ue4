#pragma once

#include "projectzCorePrivatePCH.h"
#include "projectzCoreModule.h"

bool ZProjectzCoreModule::IsGameModule() const {
    return true;
}

IMPLEMENT_MODULE(ZProjectzCoreModule, projectzCore)
