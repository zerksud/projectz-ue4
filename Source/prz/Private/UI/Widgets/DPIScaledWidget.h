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

#include "SlateBasics.h"

class SDPIScaledWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SDPIScaledWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<class ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    float GetDPIScale() const;
    void SetDPIScale(int32 scale);

    SOverlay::FOverlaySlot& AddSlot(int32 zOrder = INDEX_NONE);

private:
    TSharedPtr<SOverlay> mDPIScaledOverlay;
    int32 mScale = 1;
    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
};
