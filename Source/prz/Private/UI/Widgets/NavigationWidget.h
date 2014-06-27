#pragma once

#include "Slate.h"

class SNavigationWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SNavigationWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<class ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    float GetDPIScale() const;
    void SetDPIScale(int32 scale);

private:
    static const char* kNotificationNames[];

    int32 mScale = 1;

    FReply OnButtonClicked(int32 notificationIndex);

    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
};
