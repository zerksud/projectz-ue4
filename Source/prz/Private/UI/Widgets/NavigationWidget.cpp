// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "przPCH.h"
#include "UI/Widgets/NavigationWidget.h"

#include "Utils/ZServices.h"
#include "Utils/INotificationCenter.h"

#include "Model/ZPlayerNavigation.h"

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
