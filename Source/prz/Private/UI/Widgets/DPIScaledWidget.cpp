// Copyright 2014-2017 Max Molodtsov <zerksud@gmail.com>
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

#include "przPCH.h"
#include "UI/Widgets/DPIScaledWidget.h"

#include "Utils/LOG.h"

#include "Widgets/Layout/SDPIScaler.h"

void SDPIScaledWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    ChildSlot
    [
        SNew(SDPIScaler)
        .DPIScale(this, &SDPIScaledWidget::GetDPIScale)
        [
            SAssignNew(mDPIScaledOverlay, SOverlay)
        ]
    ];
    
}

float SDPIScaledWidget::GetDPIScale() const {
    return mScale;
}

void SDPIScaledWidget::SetDPIScale(int32 scale) {
    if (scale <= 0) {
        LOGE("Can't set non-positive scale = %d", scale);
        return;
    }

    mScale = scale;
}

SOverlay::FOverlaySlot& SDPIScaledWidget::AddSlot(int32 zorder) {
    return mDPIScaledOverlay->AddSlot(zorder);
}
