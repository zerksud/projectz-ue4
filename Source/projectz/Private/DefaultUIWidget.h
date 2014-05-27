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
private:
    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
};
