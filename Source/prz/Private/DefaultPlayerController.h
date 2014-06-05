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
    UFUNCTION()
    void MoveForwardAction();

    UFUNCTION()
    void MoveBackwardAction();

    UFUNCTION()
    void StrafeLeftAction();

    UFUNCTION()
    void StrafeRightAction();

    UFUNCTION()
    void TurnRight();

    UFUNCTION()
    void TurnLeft();

    UFUNCTION()
    void Quit();

    UFUNCTION()
    void DebugPrintCurrentLocation();

protected:
    virtual void SetupInputComponent() OVERRIDE;

private:
    void Move(EAxis::Type axis, bool reverse = false);
    void Turn(bool reversed = false);

    FVector mPlayerLocation;
    bool mPlayerLocationInitialized = false;
};
