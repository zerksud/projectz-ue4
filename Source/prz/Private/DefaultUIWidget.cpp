#include "prz.h"
#include "DefaultUIWidget.h"

void SDefaultUIWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    ChildSlot
        .VAlign(VAlign_Fill)
        .HAlign(HAlign_Fill)
        [
            SNew(SOverlay)
            + SOverlay::Slot()
            .VAlign(VAlign_Top)
            .HAlign(HAlign_Right)
            [
                SNew(STextBlock)
                .ShadowColorAndOpacity(FLinearColor::Black)
                .ColorAndOpacity(FLinearColor::White)
                .ShadowOffset(FIntPoint(-1, 1))
                .Font(FSlateFontInfo(FPaths::GameContentDir() / TEXT("Fonts/Inconsolata-Bold.ttf"), 16))
                .Text(FText::FromString(""
                "##.## \n"
                "#...# \n"
                "#.@.. \n"
                "#...# \n"
                "#.### "))
            ]
        ];
}