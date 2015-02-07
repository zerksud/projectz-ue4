#include "prz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "UI/DefaultHUD.h"

#include "Utils/Services.h"
#include "Utils/Logger.h"
#include "Utils/NotificationCenter.h"
#include "Utils/UniqueIdRegistry.h"

#include "Model/Game.h"

ADefaultGameMode::ADefaultGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    DefaultPawnClass = ADefaultPlayerCharacter::StaticClass();
    PlayerControllerClass = ADefaultPlayerController::StaticClass();
    HUDClass = ADefaultHUD::StaticClass();
}

void initializeServices() {
    prz::utl::ILogger* loggerService = new prz::utl::ZLogger();

    loggerService->SetLogCallback([](ELogVerbosity::Type verbosity, const FString& message) {
        if (GEngine && verbosity < ELogVerbosity::VeryVerbose) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, (verbosity == ELogVerbosity::Error) ? FColor::Red : FColor::Yellow, message);
        }
    });

    REGISTER_SERVICE(prz::utl::ILogger, loggerService);
    REGISTER_SERVICE(prz::utl::INotificationCenter, new prz::utl::ZNotificationCenter());
    REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new prz::utl::ZUniqueIdRegistry());
    REGISTER_SERVICE(prz::mdl::IGame, new prz::mdl::ZGame());
}

void ADefaultGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
    Super::InitGame(MapName, Options, ErrorMessage);
    initializeServices();
}

