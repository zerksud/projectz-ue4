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
}

void ADefaultPlayerCharacter::Move(EAxis::Type axis, bool reverse) {
    LOGD("Move along axis %d with reverse = %d", axis, reverse);
    if (Controller != nullptr) {
        AddMovementInput(FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(axis), reverse ? -1.0f : 1.0f);
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
        AddControllerYawInput(reverse ? -1.0f : 1.0f);
    }
}

void ADefaultPlayerCharacter::TurnRight() {
    Turn();
}

void ADefaultPlayerCharacter::TurnLeft() {
    Turn(true);
}
