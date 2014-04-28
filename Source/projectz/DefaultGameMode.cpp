#include "projectz.h"
#include "DefaultGameMode.h"


ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {

}

void ADefaultGameMode::BeginPlay() {
    Super::BeginPlay();

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Debug message"));
    }
}
