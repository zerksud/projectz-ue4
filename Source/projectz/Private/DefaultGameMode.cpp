#include "projectz.h"
#include "DefaultGameMode.h"

#include "utils/Logger.h"

ADefaultGameMode::ADefaultGameMode(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {

}

void ADefaultGameMode::BeginPlay() {
    Super::BeginPlay();

    LOGD("sample debug message %s %d %s", "with", 3, "parameters");
    LOGE("sample error message");
}
