#pragma once

#include "GameFramework/Character.h"
#include "DefaultPlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class ADefaultPlayerCharacter : public ACharacter {
    GENERATED_UCLASS_BODY()
public:
    virtual void BeginPlay() OVERRIDE;

    void Move(EAxis::Type axis, bool reverse = false);

    UFUNCTION()
    void MoveForwardAction();

    UFUNCTION()
    void MoveBackwardAction();

    UFUNCTION()
    void StrafeLeftAction();

    UFUNCTION()
    void StrafeRightAction();

    void Turn(bool reversed = false);

    UFUNCTION()
    void TurnRight();

    UFUNCTION()
    void TurnLeft();

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
};
