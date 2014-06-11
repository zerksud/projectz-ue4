#include "ModuleManager.h"

class FPrzGameModule : public FDefaultGameModuleImpl {
    virtual void StartupModule() OVERRIDE;
    virtual void ShutdownModule() OVERRIDE;
};
