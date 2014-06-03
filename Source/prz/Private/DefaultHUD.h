#pragma once

#include "GameFramework/HUD.h"
#include "DefaultHUD.generated.h"

UCLASS()
class ADefaultHUD : public AHUD {
    GENERATED_UCLASS_BODY()
public:
    TSharedPtr<class SDefaultUIWidget> DefaultUIWidget;

    void BeginPlay();
private:
    UTexture2D* mImage;
};
