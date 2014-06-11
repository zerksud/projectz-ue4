#include "prz.h"
#include "UI/Widgets/NavigationWidget.h"

#include "UI/Styles/PrzStyleManager.h"
#include "UI/Styles/NavigationWidgetStyle.h"
#include "utils/LOG_ANSI.h"

void SNavigationWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    const FNavigationStyle* style = &FPrzStyleManager::Get().GetWidgetStyle<FNavigationStyle>("NavigationWidgetStyle");

    TSharedPtr<SUniformGridPanel> gridPanel;

    ChildSlot
        .VAlign(VAlign_Bottom)
        .HAlign(HAlign_Left)
        [
            SAssignNew(gridPanel, SUniformGridPanel)
        ];

    const FSlateBrush* icons[] = {
        &style->TurnLeftButtonIcon,
        &style->MoveForwardButtonIcon,
        &style->TurnRightButtonIcon,
        &style->StrafeLeftButtonIcon,
        &style->MoveBackwardButtonIcon,
        &style->StrafeRightButtonIcon
    };

    for (int32 col = 0; col < 3; ++col) {
        for (int32 row = 0; row < 2; ++row) {
            int32 linearIndex = row * 3 + col;
            gridPanel->AddSlot(col, row)
            [
                SNew(SButton)
                .ButtonStyle(&style->ButtonStyle)
                [
                    SNew(SImage)
                    .Image(icons[linearIndex])
                ]
            ];
        }
    }
}
