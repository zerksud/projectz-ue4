#include "ModuleManager.h"

class FPrzGameModule : public FDefaultGameModuleImpl {
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
