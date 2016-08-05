// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
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

#include "prz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "UI/DefaultHUD.h"

#include "Utils/ZServices.h"
#include "Utils/ZLogger.h"
#include "Utils/ZNotificationCenter.h"
#include "Utils/ZUniqueIdRegistry.h"

#include "Model/ZGame.h"

DEFINE_LOG_CATEGORY_STATIC(ProjectZ, All, All)

ADefaultGameMode::ADefaultGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    DefaultPawnClass = ADefaultPlayerCharacter::StaticClass();
    PlayerControllerClass = ADefaultPlayerController::StaticClass();
    HUDClass = ADefaultHUD::StaticClass();
}

void initializeServices() {
    prz::utl::ILogger* loggerService = new prz::utl::ZLogger();

    loggerService->SetLogCallback([](prz::utl::ELogPriority priority, const char* fileName, int lineNum, const char* message) {
        if (GEngine && priority == prz::utl::ELogPriority::Error) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, (priority == prz::utl::ELogPriority::Error) ? FColor::Red : FColor::Yellow, message);
        }
        
		ELogVerbosity::Type verbosity = priority == prz::utl::ELogPriority::Error ? ELogVerbosity::Error : ELogVerbosity::VeryVerbose;
		FMsg::Logf(fileName, lineNum, ProjectZ.GetCategoryName(), verbosity, ANSI_TO_TCHAR(message));
    });

    REGISTER_SERVICE(prz::utl::ILogger, loggerService);
    REGISTER_SERVICE(prz::utl::INotificationCenter, new prz::utl::ZNotificationCenter());
    REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new prz::utl::ZUniqueIdRegistry());
    REGISTER_SERVICE(prz::mdl::IGame, new prz::mdl::ZGame());
}

void ADefaultGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
    Super::InitGame(MapName, Options, ErrorMessage);
    initializeServices();
}

