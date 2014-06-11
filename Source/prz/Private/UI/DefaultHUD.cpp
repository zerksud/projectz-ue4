#include "prz.h"
#include "DefaultHUD.h"

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
