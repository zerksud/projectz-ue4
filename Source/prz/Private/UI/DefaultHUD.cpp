#include "prz.h"
#include "DefaultHUD.h"

#include <algorithm>

#include "UI/Widgets/RootWidget.h"
#include "UI/Widgets/NavigationWidget.h"
#include "UI/Widgets/MinimapWidget.h"

#include "utils/Services.h"
#include "model/IGame.h"

ADefaultHUD::ADefaultHUD(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
}

void ADefaultHUD::BeginPlay() {
    SAssignNew(mRootWidget, SRootWidget).OwnerHUD(this);

    mRootWidget->AddSlot()
        .VAlign(VAlign_Bottom)
        .HAlign(HAlign_Left)
        [
            SNew(SNavigationWidget).OwnerHUD(this)
        ];

    mRootWidget->AddSlot()
        .VAlign(VAlign_Bottom)
        .HAlign(HAlign_Right)
        [
            SNew(SBox)
            .WidthOverride(29)
            .HeightOverride(29)
            [
                SAssignNew(mMinimapWidget, SMinimapWidget).OwnerHUD(this)
            ]
        ];

    if (GEngine->IsValidLowLevel()) {
        GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(mRootWidget.ToSharedRef()));
    }

    if (mRootWidget.IsValid()) {
        mRootWidget->SetVisibility(EVisibility::Visible);
    }
}

void ADefaultHUD::DrawHUD() {
    int32 sizeX, sizeY;
    GetOwningPlayerController()->GetViewportSize(sizeX, sizeY);
    static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
    float RequestedContentScaleFactor = CVar->GetFloat();
    float scaleX = sizeX / 160.0f / RequestedContentScaleFactor;
    float scaleY = sizeY / 120.0f / RequestedContentScaleFactor;

    int32 scale = std::floor(std::min(scaleX, scaleY));
    mRootWidget->SetDPIScale(scale);

    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    mMinimapWidget->SetMinimap(game->GetMinimap());

    Super::DrawHUD();
}
