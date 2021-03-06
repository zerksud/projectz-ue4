// Copyright 2014-2017 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

/**
 *
 */
UCLASS()
class ADefaultGameMode : public AGameModeBase {
    GENERATED_UCLASS_BODY()
public:
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    virtual void Logout(AController* Exiting) override;
};
