#include "prz.h"
#include "PrzGameModule.h"

#include "UI/Styles/PrzStyleManager.h"

void FPrzGameModule::StartupModule() {
    FPrzStyleManager::Startup();
}

void FPrzGameModule::ShutdownModule() {
    FPrzStyleManager::Shutdown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FPrzGameModule, prz, "prz");
