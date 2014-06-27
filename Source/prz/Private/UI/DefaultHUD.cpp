#include "prz.h"
#include "DefaultHUD.h"

#include <algorithm>

#include "UI/Widgets/NavigationWidget.h"

ADefaultHUD::ADefaultHUD(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
}

void ADefaultHUD::BeginPlay() {
    SAssignNew(mNavigationWidget, SNavigationWidget).OwnerHUD(this);

    if (GEngine->IsValidLowLevel()) {
        GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(mNavigationWidget.ToSharedRef()));
    }

    if (mNavigationWidget.IsValid()) {
        mNavigationWidget->SetVisibility(EVisibility::Visible);
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
    mNavigationWidget->SetDPIScale(scale);

    Super::DrawHUD();
}
