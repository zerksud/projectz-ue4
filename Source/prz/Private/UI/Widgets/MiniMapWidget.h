#pragma once

#include "Slate.h"

#include "model/Minimap.h"

class SMinimapWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SMinimapWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<class ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
    void Construct(const FArguments& InArgs);

    void SetMinimap(const prz::mdl::ZMinimap& minimap);

private:
    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
    TSharedPtr<prz::mdl::ZMinimap> mMinimap;
};
