#include "prz.h"
#include "DefaultPlayerController.h"

#include <map>

#include "Utils/Services.h"
#include "Utils/INotificationCenter.h"
#include "Model/IGame.h"
#include "Utils/LOG.h"

#include "Model/PlayerNavigation.h"

ADefaultPlayerController::ADefaultPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    bShowMouseCursor = 1;
    bEnableClickEvents = 1;
}

typedef void (ADefaultPlayerController::*FMethodNoParamsPtr)();

void ADefaultPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    std::map<std::string, FMethodNoParamsPtr> actionMap = {
        {"MoveForward", &ADefaultPlayerController::MoveForward},
        {"MoveBackward", &ADefaultPlayerController::MoveBackward},
        {"StrafeLeft", &ADefaultPlayerController::StrafeLeft},
        {"StrafeRight", &ADefaultPlayerController::StrafeRight},
    };

    for (auto& action : actionMap) {
        InputComponent->BindAction(action.first.c_str(), IE_Pressed, this, action.second);
        InputComponent->BindAction(action.first.c_str(), IE_Repeat, this, action.second);
    }

    InputComponent->BindAction("TurnRight", IE_Pressed, this, &ADefaultPlayerController::TurnRight);
    InputComponent->BindAction("TurnLeft", IE_Pressed, this, &ADefaultPlayerController::TurnLeft);
    InputComponent->BindAction("Quit", IE_Pressed, this, &ADefaultPlayerController::Quit);
    InputComponent->BindAction("DebugAction", IE_Pressed, this, &ADefaultPlayerController::DebugAction);

    SetupObservers();
}

void ADefaultPlayerController::SetupObservers() {
    using prz::mdl::ZPlayerNavigation;

    prz::utl::INotificationCenter* nc = GET_SERVICE(prz::utl::INotificationCenter);
    if (nc) {
        std::map<std::string, FMethodNoParamsPtr> notificationMethodMap = {
            {ZPlayerNavigation::kTurnLeftNotification, &ADefaultPlayerController::TurnLeft},
            {ZPlayerNavigation::kMoveForwardNotification, &ADefaultPlayerController::MoveForward},
            {ZPlayerNavigation::kTurnRightNotification, &ADefaultPlayerController::TurnRight},
            {ZPlayerNavigation::kStrafeLeftNotification, &ADefaultPlayerController::StrafeLeft},
            {ZPlayerNavigation::kMoveBackwardNotification, &ADefaultPlayerController::MoveBackward},
            {ZPlayerNavigation::kStrafeRightNotification, &ADefaultPlayerController::StrafeRight}
        };

        for (auto& pair : notificationMethodMap) {
            FMethodNoParamsPtr method = pair.second;
            nc->AddObserver(pair.first, this, [this, method](const prz::utl::ZDictionary& dict) {
                (this->*method)();
            });
        }
    }
}

void ADefaultPlayerController::Move(prz::mdl::ETurnDirection direction) {
    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    game->TryToMovePlayer(direction);
}

void ADefaultPlayerController::Turn(prz::mdl::ETurnDirection direction) {
    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    game->TurnPlayer(direction);
}

void ADefaultPlayerController::MoveForward() {
    Move(prz::mdl::ETurnDirection::Forward);
}

void ADefaultPlayerController::MoveBackward() {
    Move(prz::mdl::ETurnDirection::Back);
}

void ADefaultPlayerController::StrafeRight() {
    Move(prz::mdl::ETurnDirection::Right);
}

void ADefaultPlayerController::StrafeLeft() {
    Move(prz::mdl::ETurnDirection::Left);
}

void ADefaultPlayerController::TurnRight() {
    Turn(prz::mdl::ETurnDirection::Right);
}

void ADefaultPlayerController::TurnLeft() {
    Turn(prz::mdl::ETurnDirection::Left);
}

void ADefaultPlayerController::Quit() {
    ConsoleCommand("quit");
}

void ADefaultPlayerController::DebugAction() {
    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    game->StartNewGame();
}
