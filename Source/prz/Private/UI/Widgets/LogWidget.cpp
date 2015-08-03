#include "prz.h"
#include "UI/Widgets/LogWidget.h"


void SLogWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    ChildSlot
    [
        SNew(STextBlock)
        .Font(FSlateFontInfo(FPaths::GameContentDir() / TEXT("Fonts/Inconsolata-Regular.ttf"), 20))
        .Text(this, &SLogWidget::GetLogHistory)
    ];
}

void SLogWidget::UpdateLogHistory(const FText& logHistory) {
    mLogHistory = logHistory;
}

FText SLogWidget::GetLogHistory() const {
    return mLogHistory;
}