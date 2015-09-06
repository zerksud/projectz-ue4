#pragma once

#include "GameFramework/HUD.h"

#include "Model/ZMinimap.h"

#include "DefaultHUD.generated.h"

UCLASS()
class ADefaultHUD : public AHUD {
    GENERATED_UCLASS_BODY()
public:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;

protected:
    TSharedPtr<SOverlay> mRootOverlay;
    TSharedPtr<class SDPIScaledWidget> mDPIScaledWidget;
    TSharedPtr<class SMinimapWidget> mMinimapWidget;
    TSharedPtr<class SLogWidget> mLogWidget;
};
