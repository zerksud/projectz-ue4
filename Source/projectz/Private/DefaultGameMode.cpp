#include "projectz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerPawn.h"
#include "utils/Logger.h"

ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    DefaultPawnClass = ADefaultPlayerPawn::StaticClass();
}

void ADefaultGameMode::BeginPlay() {
    Super::BeginPlay();

    LOGD("DefaultGameMode created");
}
