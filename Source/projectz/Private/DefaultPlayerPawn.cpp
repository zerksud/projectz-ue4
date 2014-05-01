#include "projectz.h"
#include "DefaultPlayerPawn.h"

#include "utils/Logger.h"

ADefaultPlayerPawn::ADefaultPlayerPawn(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {

}

void ADefaultPlayerPawn::BeginPlay() {
    Super::BeginPlay();

    LOGD("DefaultPlayerPawn created");
}
