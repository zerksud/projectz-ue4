#pragma once

#include "Slate.h"

class SMiniMapWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SMiniMapWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<class ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

    virtual int32 OnPaint(const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const OVERRIDE;

public:
    void Construct(const FArguments& InArgs);

private:
    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
};
