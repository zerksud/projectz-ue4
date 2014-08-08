#pragma once

#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

/**
 *
 */
UCLASS()
class ADefaultGameMode : public AGameMode {
    GENERATED_UCLASS_BODY()
public:
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
