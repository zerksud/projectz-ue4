#pragma once

#include "GameFramework/Pawn.h"
#include "DefaultPlayerPawn.generated.h"

/**
 *
 */
UCLASS()
class ADefaultPlayerPawn : public APawn {
    GENERATED_UCLASS_BODY()
public:
    virtual void BeginPlay() OVERRIDE;
};
