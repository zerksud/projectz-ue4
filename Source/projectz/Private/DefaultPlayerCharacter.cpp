#include "projectz.h"
#include "DefaultPlayerCharacter.h"

#include "utils/Logger.h"
#include "utils/NotificationCenter.h"

ADefaultPlayerCharacter::ADefaultPlayerCharacter(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {

}

void ADefaultPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    LOGD("DefaultPlayerPawn created");
    prz::utils::ZNotificationCenter::GetInstance().AddObserver("testNotification", this, [](void*) {
        LOGD("testNotification posted");
    });
}
