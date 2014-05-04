#include "projectz.h"
#include "DefaultPlayerCharacter.h"

#include "utils/Logger.h"

ADefaultPlayerCharacter::ADefaultPlayerCharacter(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {

}

void ADefaultPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    LOGD("DefaultPlayerPawn created");
}
