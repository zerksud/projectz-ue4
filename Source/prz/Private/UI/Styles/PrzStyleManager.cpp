#include "prz.h"
#include "UI/Styles/PrzStyleManager.h"

#include "SlateGameResources.h"

TSharedPtr<FSlateStyleSet> FPrzStyleManager::sStyleSetInstance = nullptr;

void FPrzStyleManager::Startup() {
    if (!sStyleSetInstance.IsValid()) {
        sStyleSetInstance = CreateInstance();
    }

    FSlateStyleRegistry::RegisterSlateStyle(*sStyleSetInstance);
}

void FPrzStyleManager::Shutdown() {
    FSlateStyleRegistry::UnRegisterSlateStyle(GetStyleSetName());
    sStyleSetInstance.Reset();
}

FName FPrzStyleManager::GetStyleSetName() {
    return TEXT("FPrzStyleSet");
}

TSharedRef<FSlateStyleSet> FPrzStyleManager::CreateInstance() {
    TSharedRef<FSlateStyleSet> styleRef = FSlateGameResources::New(FPrzStyleManager::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");

    return styleRef;
}

const ISlateStyle& FPrzStyleManager::Get() {
    return *sStyleSetInstance;
}
