#include "prz.h"
#include "UI/Widgets/NavigationWidget.h"

#include "Utils/ZServices.h"
#include "Utils/INotificationCenter.h"

#include "Model/PlayerNavigation.h"

#include "UI/Styles/PrzStyleManager.h"
#include "UI/Styles/NavigationWidgetStyle.h"

const char* SNavigationWidget::kNotificationNames[] = {
    prz::mdl::ZPlayerNavigation::kTurnLeftNotification,
    prz::mdl::ZPlayerNavigation::kMoveForwardNotification,
    prz::mdl::ZPlayerNavigation::kTurnRightNotification,
    prz::mdl::ZPlayerNavigation::kStrafeLeftNotification,
    prz::mdl::ZPlayerNavigation::kMoveBackwardNotification,
    prz::mdl::ZPlayerNavigation::kStrafeRightNotification
};

void SNavigationWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    const FNavigationStyle* style = &FPrzStyleManager::Get().GetWidgetStyle<FNavigationStyle>("NavigationWidgetStyle");

    TSharedPtr<SUniformGridPanel> gridPanel;

    ChildSlot
    [
        SNew(SOverlay)
        + SOverlay::Slot()
        .VAlign(VAlign_Fill)
        .HAlign(HAlign_Fill)
        [
            SNew(SImage)
            .Image(&style->BackgroundImage)
        ]
        + SOverlay::Slot()
        .VAlign(VAlign_Fill)
        .HAlign(HAlign_Fill)
        [
            SAssignNew(gridPanel, SUniformGridPanel)
        ]

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
                .ContentPadding(0)
                .OnClicked(this, &SNavigationWidget::OnButtonClicked, linearIndex)
                [
                    SNew(SImage)
                    .Image(icons[linearIndex])
                ]
            ];
        }
    }
}

FReply SNavigationWidget::OnButtonClicked(int32 notificationIndex) {
    prz::utl::INotificationCenter* nc = GET_SERVICE(prz::utl::INotificationCenter);
    nc->PostNotification(kNotificationNames[notificationIndex]);

    return FReply::Handled();
}
