#include "przTestPrivatePCH.h"
#include "RequiredProgramMainCPPInclude.h"

#include "Core.h"

#include "utils/Services.h"
#include "utils/Logger.h"

IMPLEMENT_APPLICATION(przTest, "przTest");

void initializeLoggerService() {
    using namespace prz::utl;
    ZServices::GetInstance().Register<ILogger>(new ZLogger());
}

INT32_MAIN_INT32_ARGC_TCHAR_ARGV() {
    ::testing::InitGoogleTest(&ArgC, ArgV);

    initializeLoggerService();

    return RUN_ALL_TESTS();
}
