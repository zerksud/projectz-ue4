#include "prz.h"
#include "UI/Widgets/RootWidget.h"

#include "utils/LOG_ANSI.h"

void SRootWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    ChildSlot
    [
        SNew(SDPIScaler)
        .DPIScale(this, &SRootWidget::GetDPIScale)
        [
            SAssignNew(mDPIScaledOverlay, SOverlay)
        ]
    ];
    
}

float SRootWidget::GetDPIScale() const {
    return mScale;
}

void SRootWidget::SetDPIScale(int32 scale) {
    if (scale <= 0) {
        LOGE("Can't set non-positive scale = %d", scale);
        return;
    }

    mScale = scale;
}

SOverlay::FOverlaySlot& SRootWidget::AddSlot(int32 zorder) {
    return mDPIScaledOverlay->AddSlot(zorder);
}
