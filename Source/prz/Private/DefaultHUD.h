#pragma once

#include "GameFramework/HUD.h"
#include "DefaultHUD.generated.h"

typedef TSharedPtr<UTexture2D> TUTexture2DPtr;
typedef TArray<TUTexture2DPtr> TTextureArray;

UCLASS()
class ADefaultHUD : public AHUD {
    GENERATED_UCLASS_BODY()
public:
    TSharedPtr<class SDefaultUIWidget> DefaultUIWidget;

    void BeginPlay();
private:
    TTextureArray mImages;
};
