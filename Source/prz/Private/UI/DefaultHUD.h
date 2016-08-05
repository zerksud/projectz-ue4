// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
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

#include "GameFramework/HUD.h"

#include "Model/ZMinimap.h"

#include "DefaultHUD.generated.h"

UCLASS()
class ADefaultHUD : public AHUD {
    GENERATED_UCLASS_BODY()
public:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;

protected:
    TSharedPtr<SOverlay> mRootOverlay;
    TSharedPtr<class SDPIScaledWidget> mDPIScaledWidget;
    TSharedPtr<class SMinimapWidget> mMinimapWidget;
    TSharedPtr<class SLogWidget> mLogWidget;
};
