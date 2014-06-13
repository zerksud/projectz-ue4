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
    static const char* kNotificationNames[];

    FReply OnButtonClicked(int32 notificationIndex);

    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
};
