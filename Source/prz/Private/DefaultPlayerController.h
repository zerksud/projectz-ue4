// Copyright 2016 Max Molodtsov
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "GameFramework/PlayerController.h"

#include "Model/IGame.h"

#include "DefaultPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ADefaultPlayerController : public APlayerController {
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION()
    void MoveForward();

    UFUNCTION()
    void MoveBackward();

    UFUNCTION()
    void StrafeLeft();

    UFUNCTION()
    void StrafeRight();

    UFUNCTION()
    void TurnRight();

    UFUNCTION()
    void TurnLeft();

    UFUNCTION()
    void Quit();

    UFUNCTION()
    void DebugAction();

protected:
    virtual void SetupInputComponent() override;

private:
    void SetupObservers();

    void Move(prz::mdl::EDirection direction);
    void Turn(prz::mdl::EDirection direction);
};
