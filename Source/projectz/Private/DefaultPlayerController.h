#pragma once

#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ADefaultPlayerController : public APlayerController {
    GENERATED_UCLASS_BODY()
public:
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

    UFUNCTION()
    void DebugPrintCurrentLocation();

protected:
    virtual void SetupInputComponent() OVERRIDE;
};
