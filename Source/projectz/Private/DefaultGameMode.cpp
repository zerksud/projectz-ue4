#include "projectz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "utils/Logger.h"

ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    DefaultPawnClass = ADefaultPlayerCharacter::StaticClass();
}

void ADefaultGameMode::BeginPlay() {
    Super::BeginPlay();

    LOGD("DefaultGameMode created");
}
