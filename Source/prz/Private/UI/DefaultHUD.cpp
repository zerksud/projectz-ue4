#include "prz.h"
#include "DefaultHUD.h"

#include "Utils/StandartLibrary/Algorithm.h"
#include "Utils/StandartLibrary/Math.h"
#include "Utils/Services.h"

#include "Model/IGame.h"

#include "UI/Widgets/DPIScaledWidget.h"
#include "UI/Widgets/NavigationWidget.h"
#include "UI/Widgets/MinimapWidget.h"
#include "UI/Widgets/LogWidget.h"

ADefaultHUD::ADefaultHUD(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer) {
}

void ADefaultHUD::BeginPlay() {
    SAssignNew(mRootOverlay, SOverlay)
    + SOverlay::Slot()
    [
        SAssignNew(mDPIScaledWidget, SDPIScaledWidget).OwnerHUD(this)
    ]
    + SOverlay::Slot()
    .VAlign(VAlign_Top)
    .HAlign(HAlign_Left)
    [
        SAssignNew(mLogWidget, SLogWidget).OwnerHUD(this)
    ];

    mDPIScaledWidget->AddSlot()
        .VAlign(VAlign_Bottom)
        .HAlign(HAlign_Left)
        [
            SNew(SNavigationWidget).OwnerHUD(this)
        ];

    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    unsigned int minimapSize = game->GetMinimapSize();
    mDPIScaledWidget->AddSlot()
        .VAlign(VAlign_Bottom)
        .HAlign(HAlign_Right)
        [
            SNew(SBox)
            .WidthOverride(minimapSize)
            .HeightOverride(minimapSize)
            [
                SAssignNew(mMinimapWidget, SMinimapWidget).OwnerHUD(this)
            ]
        ];

    if (GEngine->IsValidLowLevel()) {
        GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(mRootOverlay.ToSharedRef()));
    }

    if (mRootOverlay.IsValid()) {
        mRootOverlay->SetVisibility(EVisibility::Visible);
    }
}

void ADefaultHUD::DrawHUD() {
    int32 sizeX, sizeY;
    GetOwningPlayerController()->GetViewportSize(sizeX, sizeY);
    static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
    float RequestedContentScaleFactor = CVar->GetFloat();
    float scaleX = sizeX / 120.0f / RequestedContentScaleFactor;
    float scaleY = sizeY / 90.0f / RequestedContentScaleFactor;

    int32 scale = prz::utl::floor(prz::utl::min(scaleX, scaleY));
    mDPIScaledWidget->SetDPIScale(scale);

    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    mMinimapWidget->SetMinimap(game->GetMinimap());

    mLogWidget->UpdateLogHistory(FText::FromString(game->GetLogHistory().c_str()));

    Super::DrawHUD();
}
