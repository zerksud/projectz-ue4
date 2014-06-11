#pragma once

#include "Slate.h"

class SNavigationWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SNavigationWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<class ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

private:
    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
};
