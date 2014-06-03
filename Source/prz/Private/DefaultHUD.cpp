#include "prz.h"
#include "DefaultHUD.h"

#include "DefaultUIWidget.h"

ADefaultHUD::ADefaultHUD(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    static ConstructorHelpers::FObjectFinder<UTexture2D> ImageObj(TEXT("/Game/UI/ForwardButton"));
    mImage = ImageObj.Object;
}

void ADefaultHUD::BeginPlay() {
    SAssignNew(DefaultUIWidget, SDefaultUIWidget).OwnerHUD(this);

    DefaultUIWidget->SetImage(mImage);

    if (GEngine->IsValidLowLevel()) {
        GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(DefaultUIWidget.ToSharedRef()));
    }

    if (DefaultUIWidget.IsValid()) {
        DefaultUIWidget->SetVisibility(EVisibility::Visible);
    }
}
