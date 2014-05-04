#pragma once

#include "GameFramework/Character.h"
#include "DefaultPlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class ADefaultPlayerCharacter : public ACharacter {
    GENERATED_UCLASS_BODY()
public:
    virtual void BeginPlay() OVERRIDE;
};
