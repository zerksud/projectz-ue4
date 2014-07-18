#include "prz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "UI/DefaultHUD.h"

#include "utils/Services.h"
#include "utils/Logger.h"
#include "utils/NotificationCenter.h"
#include "utils/UniqueIdRegistry.h"

#include "model/Game.h"

void initializeServices() {
    using namespace prz::utl;

    ILogger* loggerService = new ZLogger();

    loggerService->SetLogCallback([](ELogVerbosity::Type verbosity, const FString& message) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, (verbosity == ELogVerbosity::Error) ? FColor::Red : FColor::Yellow, message);
        }
    });

    REGISTER_SERVICE(prz::utl::ILogger, loggerService);
    REGISTER_SERVICE(prz::utl::INotificationCenter, new prz::utl::ZNotificationCenter());
    REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new prz::utl::ZUniqueIdRegistry());
    REGISTER_SERVICE(prz::mdl::IGame, new prz::mdl::ZGame());
}

ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    initializeServices();

    DefaultPawnClass = ADefaultPlayerCharacter::StaticClass();
    PlayerControllerClass = ADefaultPlayerController::StaticClass();
    HUDClass = ADefaultHUD::StaticClass();
}
