#include "prz.h"
#include "DefaultGameMode.h"

#include "DefaultPlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "UI/DefaultHUD.h"

#include "Utils/Services.h"
#include "Utils/Logger.h"
#include "Utils/NotificationCenter.h"
#include "Utils/UniqueIdRegistry.h"

#include "Model/Game.h"

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

