#include "projectz.h"
#include "DefaultHUD.h"

#include "DefaultUIWidget.h"

ADefaultHUD::ADefaultHUD(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {

}

void ADefaultHUD::BeginPlay() {
    SAssignNew(DefaultUIWidget, SDefaultUIWidget).OwnerHUD(this);

    if (GEngine->IsValidLowLevel()) {
        GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(DefaultUIWidget.ToSharedRef()));
    }

    if (DefaultUIWidget.IsValid()) {
        DefaultUIWidget->SetVisibility(EVisibility::Visible);
    }
}
