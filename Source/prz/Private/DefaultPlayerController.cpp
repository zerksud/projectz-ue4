#include "prz.h"
#include "DefaultPlayerController.h"

#include <map>

#include "utils/Services.h"
#include "utils/INotificationCenter.h"
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
    InputComponent->BindAction("DebugPrintCurrentLocation", IE_Pressed, this, &ADefaultPlayerController::DebugPrintCurrentLocation);

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

void ADefaultPlayerController::Move(EAxis::Type axis, bool reverse) {
    APawn* pawn = GetPawn();
    if (pawn) {
        if (!mPlayerLocationInitialized) {
            mPlayerLocation = pawn->GetActorLocation();
            mPlayerLocationInitialized = true;
        }

        FVector moveDistance = 500.0f * (reverse ? -1.0f : 1.0f) * FRotationMatrix(GetControlRotation()).GetScaledAxis(axis);

        FVector destination = mPlayerLocation + moveDistance;
        LOGD("destination: %s", *destination.ToString());

        UNavigationComponent* navComp = nullptr;
        UPathFollowingComponent* pathComp = nullptr;

        InitNavigationControl(navComp, pathComp);
        if (navComp && pathComp && navComp->FindPathToLocation(destination)) {
            mPlayerLocation += moveDistance;
            pathComp->RequestMove(navComp->GetPath(), nullptr, 0.0f, false);
        }
    }
}

void ADefaultPlayerController::Turn(bool reverse) {
    FRotator rotation = GetControlRotation();
    rotation.Yaw += (reverse ? -1.0f : 1.0f) * 90.0f;
    SetControlRotation(rotation);
}

void ADefaultPlayerController::MoveForwardAction() {
    Move(EAxis::X);
}

void ADefaultPlayerController::MoveBackwardAction() {
    Move(EAxis::X, true);
}

void ADefaultPlayerController::StrafeRightAction() {
    Move(EAxis::Y);
}

void ADefaultPlayerController::StrafeLeftAction() {
    Move(EAxis::Y, true);
}

void ADefaultPlayerController::TurnRight() {
    Turn();
}

void ADefaultPlayerController::TurnLeft() {
    Turn(true);
}

void ADefaultPlayerController::Quit() {
    ConsoleCommand("quit");
}

void ADefaultPlayerController::DebugPrintCurrentLocation() {
    APawn* pawn = GetPawn();
    if (pawn) {
        LOGD("current location: %s", *pawn->GetActorLocation().ToString());
    }
}
