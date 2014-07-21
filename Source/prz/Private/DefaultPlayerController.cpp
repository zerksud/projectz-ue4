#include "prz.h"
#include "DefaultPlayerController.h"

#include <map>

#include "utils/Services.h"
#include "utils/INotificationCenter.h"
#include "model/IGame.h"
#include "utils/LOG.h"

#include "model/PlayerNavigation.h"

ADefaultPlayerController::ADefaultPlayerController(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP) {
    bShowMouseCursor = 1;
    bEnableClickEvents = 1;
}

void ADefaultPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    InputComponent->BindAction("MoveForward", IE_Pressed, this, &ADefaultPlayerController::MoveForwardAction);
    InputComponent->BindAction("MoveBackward", IE_Pressed, this, &ADefaultPlayerController::MoveBackwardAction);
    InputComponent->BindAction("StrafeLeft", IE_Pressed, this, &ADefaultPlayerController::StrafeLeftAction);
    InputComponent->BindAction("StrafeRight", IE_Pressed, this, &ADefaultPlayerController::StrafeRightAction);
    InputComponent->BindAction("TurnRight", IE_Pressed, this, &ADefaultPlayerController::TurnRight);
    InputComponent->BindAction("TurnLeft", IE_Pressed, this, &ADefaultPlayerController::TurnLeft);
    InputComponent->BindAction("Quit", IE_Pressed, this, &ADefaultPlayerController::Quit);
    InputComponent->BindAction("DebugPrintCurrentLocation", IE_Pressed, this, &ADefaultPlayerController::DebugAction);

    SetupObservers();
}

void ADefaultPlayerController::SetupObservers() {
    using namespace prz::utl;
    using namespace prz::mdl;

    INotificationCenter* nc = GET_SERVICE(prz::utl::INotificationCenter);
    if (nc) {
        typedef void (ADefaultPlayerController::*FMethodNoParamsPtr)();

        std::map<std::string, FMethodNoParamsPtr> notificationMethodMap = {
            {ZPlayerNavigation::kTurnLeftNotification, &ADefaultPlayerController::TurnLeft},
            {ZPlayerNavigation::kMoveForwardNotification, &ADefaultPlayerController::MoveForwardAction},
            {ZPlayerNavigation::kTurnRightNotification, &ADefaultPlayerController::TurnRight},
            {ZPlayerNavigation::kStrafeLeftNotification, &ADefaultPlayerController::StrafeLeftAction},
            {ZPlayerNavigation::kMoveBackwardNotification, &ADefaultPlayerController::MoveBackwardAction},
            {ZPlayerNavigation::kStrafeRightNotification, &ADefaultPlayerController::StrafeRightAction}
        };

        for (auto& pair : notificationMethodMap) {
            FMethodNoParamsPtr method = pair.second;
            nc->AddObserver(pair.first, this, [this, method](const ZDictionary& dict) {
                (this->*method)();
            });
        }
    }
}

void ADefaultPlayerController::Move(prz::mdl::EMoveDirection::Type direction) {
    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    game->TryToMovePlayer(direction);
}

void ADefaultPlayerController::Turn(prz::mdl::ETurnDirection::Type direction) {
    prz::mdl::IGame* game = GET_SERVICE(prz::mdl::IGame);
    game->TurnPlayer(direction);
}

void ADefaultPlayerController::MoveForwardAction() {
    Move(prz::mdl::EMoveDirection::Forward);
}

void ADefaultPlayerController::MoveBackwardAction() {
    Move(prz::mdl::EMoveDirection::Backward);
}

void ADefaultPlayerController::StrafeRightAction() {
    Move(prz::mdl::EMoveDirection::Right);
}

void ADefaultPlayerController::StrafeLeftAction() {
    Move(prz::mdl::EMoveDirection::Left);
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
    LOGD("Debug action");
}
