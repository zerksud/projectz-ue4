#include "projectzTestPrivatePCH.h"

#include "gtest/gtest.h"

#include "model/Dungeon.h"

#define ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeon) \
    EXPECT_EQ(kFailSafeMapWidth, dungeon.GetWidth()); \
    EXPECT_EQ(kFailSafeMapHeight, dungeon.GetHeight()); \
    ASSERT_TRUE(dungeon.CellIsEmpty(kFailSafeMapEmptyCell));

namespace prz {
    namespace testing {
        class DungeonTest : public ::testing::Test {
        protected:
            const static mdl::ZDungeon::ZMapCell* kSomeMap;
            const static int kSomeMapWidth = 5;
            const static int kSomeMapHeight = 3;
            const static mdl::ZPosition kSomeMapSomeEmptyCell;
            const static mdl::ZPosition kSomeMapSomeSolidCell;

            const static int kSomeIncorrectMapWidth = 0;
            const static int kSomeIncorrectMapHeight = -5;

            const static int kFailSafeMapWidth = 3;
            const static int kFailSafeMapHeight = 3;
            const static mdl::ZPosition kFailSafeMapEmptyCell;
        };

        const mdl::ZDungeon::ZMapCell* DungeonTest::kSomeMap = ""
            "#####"
            "#<.>#"
            "#####";
        const mdl::ZPosition DungeonTest::kSomeMapSomeEmptyCell = mdl::ZPosition(1, 2);
        const mdl::ZPosition DungeonTest::kSomeMapSomeSolidCell = mdl::ZPosition(2, 1);

        const mdl::ZPosition DungeonTest::kFailSafeMapEmptyCell = mdl::ZPosition(1, 1);

        class DungeonWithSomeMapTest : public DungeonTest {
        protected:
            void SetUp() {
                mDungeon = new mdl::ZDungeon(kSomeMapWidth, kSomeMapHeight, kSomeMap);
            }

            void TearDown() {
                delete mDungeon;
            }

            mdl::ZDungeon* mDungeon;
        };

        TEST_F(DungeonWithSomeMapTest, GetWidthAndGetHeightReturnExpectedValues) {
            EXPECT_EQ(kSomeMapWidth, mDungeon->GetWidth());
            ASSERT_EQ(kSomeMapHeight, mDungeon->GetHeight());
        }

        TEST_F(DungeonTest, IncorrectWidthCausesCreationOfDungeonWithFailSafeMap) {
            mdl::ZDungeon dungeon(kSomeIncorrectMapWidth, kSomeMapHeight, kSomeMap);

            ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeon);
        }

        TEST_F(DungeonTest, IncorrectHeightCausesCreationOfDungeonWithFailSafeMap) {
            mdl::ZDungeon dungeon(kSomeMapWidth, kSomeIncorrectMapHeight, kSomeMap);

            ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeon);
        }

        TEST_F(DungeonTest, MapWithSizeInconsistentWithWidthAndHeightCausesCreationOfDungeonWithFailSafeMap) {
            mdl::ZDungeon dungeon(kSomeMapWidth, kSomeMapHeight + 1, kSomeMap);

            ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeon);
        }
    }
}
