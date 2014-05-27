#include "projectz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "DefaultHUD.h"

#include "utils/LOG.h"
#include "utils/Logger.h"
#include "utils/NotificationCenter.h"

ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    DefaultPawnClass = ADefaultPlayerCharacter::StaticClass();
    PlayerControllerClass = ADefaultPlayerController::StaticClass();
    HUDClass = ADefaultHUD::StaticClass();
}

void initializeServices() {
    using namespace prz::utils;

    ILogger* loggerService = new ZLogger();

    loggerService->SetLogCallback([](ELogVerbosity::Type verbosity, const FString& message) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, (verbosity == ELogVerbosity::Error) ? FColor::Red : FColor::Yellow, message);
        }
    });

    ZServices::GetInstance().Register<ILogger>(loggerService);
    ZServices::GetInstance().Register<INotificationCenter>(new prz::utils::ZNotificationCenter());
}

void ADefaultGameMode::BeginPlay() {
    Super::BeginPlay();

    initializeServices();

    LOGD("DefaultGameMode created");
}
