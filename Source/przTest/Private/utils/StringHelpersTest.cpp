#include "przTestPrivatePCH.h"

#include "utils/StringHelpers.h"

namespace prz {
    namespace testing {
        
        TEST(StringHelpersTest, Format_FormatsString) {
            ASSERT_EQ("some string with 1 number", utl::ZString::Format("some %s with %d number", "string", 1));
        }

    }
}
