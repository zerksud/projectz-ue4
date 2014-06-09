#pragma once

#include "Slate.h"

#include "DefaultHUD.h"

class SDefaultUIWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SDefaultUIWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    void SetImages(TTextureArray& images);

protected:
    const FSlateBrush*  GetImage(const int32 index) const;
private:
    FReply OnButtonClicked(int32 buttonIndex);

    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;

    typedef TArray<TSharedPtr<FSlateDynamicImageBrush> > TImageBrushArray;
    TImageBrushArray mImages;
};
