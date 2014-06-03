#pragma once

#include "Slate.h"

#include "DefaultHUD.h"

UCLASS()
class SDefaultUIWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SDefaultUIWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    void SetImage(UTexture2D* image);

protected:
    const FSlateBrush*  GetImage() const;
private:
    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;

    TSharedPtr<FSlateDynamicImageBrush> mImage;
};
