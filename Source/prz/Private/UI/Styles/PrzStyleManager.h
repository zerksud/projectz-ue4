#pragma once

#include "SlateBasics.h"

class FPrzStyleManager {
public:
    static void Startup();
    static void Shutdown();

    static const ISlateStyle& Get();

private:
    static FName GetStyleSetName();
    static TSharedRef<FSlateStyleSet> CreateInstance();
    static TSharedPtr<FSlateStyleSet> sStyleSetInstance;
};
