#pragma once

#include <string>

#include "SlateBasics.h"

class SLogWidget : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SLogWidget) {
    }
    SLATE_ARGUMENT(TWeakObjectPtr<class ADefaultHUD>, OwnerHUD)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    void UpdateLogHistory(const FText& logHistory);
    FText GetLogHistory() const;

private:
    TWeakObjectPtr<class ADefaultHUD> OwnerHUD;
    FText mLogHistory;
};
