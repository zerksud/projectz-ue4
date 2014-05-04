#include "projectz.h"
#include "DefaultPlayerController.h"

#include "utils/Logger.h"

ADefaultPlayerController::ADefaultPlayerController(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {

}

void ADefaultPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    InputComponent->BindAction("MoveForward", IE_Pressed, this, &ADefaultPlayerController::MoveForwardAction);
    InputComponent->BindAction("MoveBackward", IE_Pressed, this, &ADefaultPlayerController::MoveBackwardAction);
    InputComponent->BindAction("StrafeLeft", IE_Pressed, this, &ADefaultPlayerController::StrafeLeftAction);
    InputComponent->BindAction("StrafeRight", IE_Pressed, this, &ADefaultPlayerController::StrafeRightAction);
    InputComponent->BindAction("TurnRight", IE_Pressed, this, &ADefaultPlayerController::TurnRight);
    InputComponent->BindAction("TurnLeft", IE_Pressed, this, &ADefaultPlayerController::TurnLeft);
    InputComponent->BindAction("DebugPrintCurrentLocation", IE_Pressed, this, &ADefaultPlayerController::DebugPrintCurrentLocation);
}

void ADefaultPlayerController::Move(EAxis::Type axis, bool reverse) {
    LOGD("Move along axis %d with reverse = %d", axis, reverse);

    APawn* pawn = GetPawn();
    if (pawn) {
        FVector moveDistance = 100.0f * (reverse ? -1.0f : 1.0f) * FRotationMatrix(GetControlRotation()).GetScaledAxis(axis);
        LOGD("move direction: %s", TCHAR_TO_ANSI(*moveDistance.ToString()));

        FVector destination = pawn->GetActorLocation() + moveDistance;
        LOGD("destination: %s", TCHAR_TO_ANSI(*destination.ToString()));

        UNavigationComponent* navComp = nullptr;
        UPathFollowingComponent* pathComp = nullptr;

        InitNavigationControl(navComp, pathComp);
        if (navComp && pathComp && navComp->FindPathToLocation(destination)) {
            pathComp->RequestMove(navComp->GetPath(), nullptr, 0.0f, false);
        }
    }
}

void ADefaultPlayerController::MoveForwardAction() {
    Move(EAxis::X);
}

void ADefaultPlayerController::MoveBackwardAction() {
    Move(EAxis::X, true);
}

void ADefaultPlayerController::StrafeRightAction() {
    Move(EAxis::Y);
}

void ADefaultPlayerController::StrafeLeftAction() {
    Move(EAxis::Y, true);
}

void ADefaultPlayerController::Turn(bool reverse) {
    LOGD("Turn with reverse = %d", reverse);

    FRotator rotation = GetControlRotation();
    rotation.Yaw += (reverse ? -1.0f : 1.0f) * 90.0f;
    SetControlRotation(rotation);
}

void ADefaultPlayerController::TurnRight() {
    Turn();
}

void ADefaultPlayerController::TurnLeft() {
    Turn(true);
}

void ADefaultPlayerController::DebugPrintCurrentLocation() {
    APawn* pawn = GetPawn();
    if (pawn) {
        LOGD("current location: %s", TCHAR_TO_ANSI(*pawn->GetActorLocation().ToString()));
    }
}