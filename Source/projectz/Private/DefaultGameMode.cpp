#include "projectz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "utils/Logger.h"

ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    DefaultPawnClass = ADefaultPlayerCharacter::StaticClass();
    PlayerControllerClass = ADefaultPlayerController::StaticClass();
}

void ADefaultGameMode::BeginPlay() {
    Super::BeginPlay();

    prz::utils::ZLogger::GetInstance().SetLogCallback([](ELogVerbosity::Type verbosity, const FString& message) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, (verbosity == ELogVerbosity::Error) ? FColor::Red : FColor::Yellow, message);
        }
    });

    LOGD("DefaultGameMode created");
}
