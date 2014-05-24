#include "projectzTestPrivatePCH.h"

#include "gtest/gtest.h"

#include "model/Dungeon.h"

namespace prz {
    namespace testing {
        class DungeonTest : public ::testing::Test {
        protected:
            const static mdl::ZDungeon::ZMapCell* kSomeMap;
            const static int kSomeMapWidth = 5;
            const static int kSomeMapHeight = 3;
            const static mdl::ZPosition kSomeMapSomeEmptyCell;
            const static mdl::ZPosition kSomeMapSomeSolidCell;
        };

        const mdl::ZDungeon::ZMapCell* DungeonTest::kSomeMap = ""
            "#####"
            "#<.>#"
            "#####";
        const mdl::ZPosition DungeonTest::kSomeMapSomeEmptyCell = mdl::ZPosition(1, 2);
        const mdl::ZPosition DungeonTest::kSomeMapSomeSolidCell = mdl::ZPosition(2, 1);
            
        TEST_F(DungeonTest, GetWidthAndGetHeightReturnExpectedValues) {
            mdl::ZDungeon dungeon(kSomeMapWidth, kSomeMapHeight, kSomeMap);

            EXPECT_EQ(kSomeMapWidth, dungeon.GetWidth());
            ASSERT_EQ(kSomeMapHeight, dungeon.GetHeight());
        }
    }
}
