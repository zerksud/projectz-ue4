#include "przTestPrivatePCH.h"

#include "model/Game.h"

#include "utils/Services.h"
#include "utils/UniqueIdRegistry.h"

namespace prz {
    namespace testing {

        class GameTestWithIdRegistry : public ::testing::Test {
            protected:
                virtual void SetUp() override {
                    REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new prz::utl::ZUniqueIdRegistry());
                }

                virtual void TearDown() override {
                    UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);
                }
        };

        TEST(GameTestWithIdRegistry, Constructor_SimplyWorks) {
            mdl::ZGame game;
        }

    }
}
