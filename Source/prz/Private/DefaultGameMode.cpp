#include "prz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "UI/DefaultHUD.h"

#include "utils/LOG.h"
#include "utils/Logger.h"
#include "utils/NotificationCenter.h"

void initializeServices() {
    using namespace prz::utl;

    ILogger* loggerService = new ZLogger();

    loggerService->SetLogCallback([](ELogVerbosity::Type verbosity, const FString& message) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, (verbosity == ELogVerbosity::Error) ? FColor::Red : FColor::Yellow, message);
        }
    });

    ZServices::GetInstance().Register<ILogger>(loggerService);
    ZServices::GetInstance().Register<INotificationCenter>(new prz::utl::ZNotificationCenter());
}

ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    initializeServices();

    DefaultPawnClass = ADefaultPlayerCharacter::StaticClass();
    PlayerControllerClass = ADefaultPlayerController::StaticClass();
    HUDClass = ADefaultHUD::StaticClass();
}

void ADefaultGameMode::BeginPlay() {
    Super::BeginPlay();

    LOGD("DefaultGameMode created");
}
