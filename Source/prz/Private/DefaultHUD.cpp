#include "prz.h"
#include "DefaultHUD.h"

#include "DefaultUIWidget.h"

ADefaultHUD::ADefaultHUD(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    const char* pathPrefix = "/Game/UI/HUD/";
    const char* buttonsTextures[] = {"TurnLeft", "MoveForward", "TurnRight", "StrafeLeft", "MoveBackward", "StrafeRight"};

    for (int8 i = 0; i < 6; ++i) {
        FString texturePath = FString::Printf(TEXT("%s%s"), ANSI_TO_TCHAR(pathPrefix), ANSI_TO_TCHAR(buttonsTextures[i]));
        ConstructorHelpers::FObjectFinder<UTexture2D> imageObj(*texturePath);
        mImages.Add(UTexture2DPtr(imageObj.Object));
    }
}

void ADefaultHUD::BeginPlay() {
    SAssignNew(DefaultUIWidget, SDefaultUIWidget).OwnerHUD(this);

    DefaultUIWidget->SetImages(mImages);

    if (GEngine->IsValidLowLevel()) {
        GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(DefaultUIWidget.ToSharedRef()));
    }

    if (DefaultUIWidget.IsValid()) {
        DefaultUIWidget->SetVisibility(EVisibility::Visible);
    }
}
