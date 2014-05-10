#include "projectzTest.h"
#include "RequiredProgramMainCPPInclude.h"

#include "gtest/gtest.h"

IMPLEMENT_APPLICATION(projectzTest, "projectzTest");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV() {
    ::testing::InitGoogleTest(&ArgC, ArgV);
    return RUN_ALL_TESTS();
}
