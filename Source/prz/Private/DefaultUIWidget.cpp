#include "prz.h"
#include "DefaultUIWidget.h"

#include "utils/LOG_ANSI.h"

void SDefaultUIWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    TSharedPtr<SUniformGridPanel> navigationPanel;

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
            + SOverlay::Slot()
            .VAlign(VAlign_Bottom)
            .HAlign(HAlign_Left)
            [
                SAssignNew(navigationPanel, SUniformGridPanel)
            ]
        ];

    int32 rowCount = 2;
    int32 colCount = 3;
    for (int32 row = 0; row < rowCount; ++row) {
        for (int32 col = 0; col < colCount; ++col) {
            int32 linearButtonIndex = row * colCount + col;
            navigationPanel->AddSlot(col, row)
            [
                SNew(SButton)
                .ButtonStyle(FCoreStyle::Get(), "NoBorder")
                .OnClicked(this, &SDefaultUIWidget::OnButtonClicked, linearButtonIndex)
                [
                    SNew(SImage).Image(this, &SDefaultUIWidget::GetImage, linearButtonIndex)
                ]
            ];
        }
    }
}

FReply SDefaultUIWidget::OnButtonClicked(int32 buttonIndex) {
    LOGD("#%d button clicked", buttonIndex);

    return FReply::Handled();
}

const FSlateBrush*  SDefaultUIWidget::GetImage(const int32 index) const {
    if (mImages[index].IsValid()) {
        return mImages[index].Get();
    }

    return nullptr;
}

void SDefaultUIWidget::SetImages(TTextureArray& images) {
    mImages.Reset();
    for (int32 i = 0; i < images.Num(); ++i) {
        UTexture2D* image = images[i];
        FSlateDynamicImageBrush* brush = new FSlateDynamicImageBrush(image, FVector2D(image->GetSizeX(), image->GetSizeY()), image->GetFName());
        mImages.Add(TSharedPtr<FSlateDynamicImageBrush>(brush));
    }
}
