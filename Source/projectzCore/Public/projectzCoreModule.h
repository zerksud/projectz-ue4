#pragma once

#include "ModuleManager.h"

class ZProjectzCoreModule : public IModuleInterface {
    virtual bool IsGameModule() const OVERRIDE;
};
