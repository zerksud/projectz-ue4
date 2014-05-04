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

void ADefaultPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
    InputComponent->BindAction("MoveForward", IE_Pressed, this, &ADefaultPlayerCharacter::MoveForwardAction);
    InputComponent->BindAction("MoveBackward", IE_Pressed, this, &ADefaultPlayerCharacter::MoveBackwardAction);
    InputComponent->BindAction("StrafeLeft", IE_Pressed, this, &ADefaultPlayerCharacter::StrafeLeftAction);
    InputComponent->BindAction("StrafeRight", IE_Pressed, this, &ADefaultPlayerCharacter::StrafeRightAction);
    InputComponent->BindAction("TurnRight", IE_Pressed, this, &ADefaultPlayerCharacter::TurnRight);
    InputComponent->BindAction("TurnLeft", IE_Pressed, this, &ADefaultPlayerCharacter::TurnLeft);
    InputComponent->BindAction("DebugPrintCurrentLocation", IE_Pressed, this, &ADefaultPlayerCharacter::DebugPrintCurrentLocation);
}

void ADefaultPlayerCharacter::Move(EAxis::Type axis, bool reverse) {
    LOGD("Move along axis %d with reverse = %d", axis, reverse);
    if (Controller != nullptr) {
        FVector moveDistance = 100.0f * (reverse ? -1.0f : 1.0f) * FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(axis);
        LOGD("move direction: %s", TCHAR_TO_ANSI(*moveDistance.ToString()));

        FVector destination = GetActorLocation() + moveDistance;
        LOGD("destination: %s", TCHAR_TO_ANSI(*destination.ToString()));

        UNavigationComponent* navComp = nullptr;
        UPathFollowingComponent* pathComp = nullptr;

        Controller->InitNavigationControl(navComp, pathComp);
        if (navComp && pathComp && navComp->FindPathToLocation(destination)) {
            pathComp->RequestMove(navComp->GetPath(), nullptr, 0.0f, false);
        }
    }
}

void ADefaultPlayerCharacter::MoveForwardAction() {
    Move(EAxis::X);
}

void ADefaultPlayerCharacter::MoveBackwardAction() {
    Move(EAxis::X, true);
}

void ADefaultPlayerCharacter::StrafeRightAction() {
    Move(EAxis::Y);
}

void ADefaultPlayerCharacter::StrafeLeftAction() {
    Move(EAxis::Y, true);
}

void ADefaultPlayerCharacter::Turn(bool reverse) {
    LOGD("Turn with reverse = %d", reverse);
    if (Controller != nullptr) {
        FRotator rotation = Controller->GetControlRotation();
        rotation.Yaw += (reverse ? -1.0f : 1.0f) * 90.0f;
        Controller->SetControlRotation(rotation);
    }
}

void ADefaultPlayerCharacter::TurnRight() {
    Turn();
}

void ADefaultPlayerCharacter::TurnLeft() {
    Turn(true);
}

void ADefaultPlayerCharacter::DebugPrintCurrentLocation() {
    LOGD("current location: %s", TCHAR_TO_ANSI(*GetActorLocation().ToString()));
}
