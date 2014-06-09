#pragma once

#include "GameFramework/HUD.h"
#include "DefaultHUD.generated.h"

typedef TArray<UTexture2D*> TTextureArray;

UCLASS()
class ADefaultHUD : public AHUD {
    GENERATED_UCLASS_BODY()
public:
    TSharedPtr<class SDefaultUIWidget> DefaultUIWidget;

    void BeginPlay();
private:
    TTextureArray mImages;
};
