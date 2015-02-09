#include "prz.h"
#include "UI/Widgets/DPIScaledWidget.h"

#include "Widgets/Layout/SDPIScaler.h"

#include "Utils/LOG.h"

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
