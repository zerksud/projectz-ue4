#include "projectzTestPrivatePCH.h"

#include "model/Dungeon.h"

#include "model/PositionTest.h"
#include "TestHelpers.h"

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
            const static mdl::ZPosition kSomeMapSomeHollowCell;
            const static mdl::ZPosition kSomeMapSomeSolidCell;
            const static mdl::ZPosition kSomeMapStairsUpCell;
            const static mdl::ZPosition kSomeMapStairsDownCell;

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
        const mdl::ZPosition DungeonTest::kSomeMapSomeHollowCell = mdl::ZPosition(2, 1);
        const mdl::ZPosition DungeonTest::kSomeMapSomeSolidCell = mdl::ZPosition(1, 2);
        const mdl::ZPosition DungeonTest::kSomeMapStairsUpCell = mdl::ZPosition(1, 1);
        const mdl::ZPosition DungeonTest::kSomeMapStairsDownCell = mdl::ZPosition(3, 1);

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

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_HollowCellsAreNotSolid) {
            ASSERT_FALSE(mDungeon->CellIsSolid(kSomeMapSomeHollowCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_StairsUpCellsAreNotSolid) {
            ASSERT_FALSE(mDungeon->CellIsSolid(kSomeMapStairsUpCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_StairsDownCellsAreNotSolid) {
            ASSERT_FALSE(mDungeon->CellIsSolid(kSomeMapStairsDownCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_SolidCellsAreIndeedSolid) {
            ASSERT_TRUE(mDungeon->CellIsSolid(kSomeMapSomeSolidCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_CoordinatesWorksJustLikePosition) {
            ASSERT_TRUE(mDungeon->CellIsSolid(kSomeMapSomeSolidCell.GetX(), kSomeMapSomeSolidCell.GetY()));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_HollowCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeMapSomeHollowCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_StairsUpCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeMapStairsUpCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_StairsDownCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeMapStairsDownCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_SolidCellsAreNotEmpty) {
            ASSERT_FALSE(mDungeon->CellIsEmpty(kSomeMapSomeSolidCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_CoordinatesWorksJustLikePosition) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeMapSomeHollowCell.GetX(), kSomeMapSomeHollowCell.GetY()));
        }

        class DungeonSingleStairsDownTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kStairsDownPosition;

            void SetUp() {
                mDungeon = new mdl::ZDungeon(4, 3, ""
                "####"
                "#.>#"
                "####");
            }

            void TearDown() {
                delete mDungeon;
            }

            mdl::ZDungeon* mDungeon;
        };

        const mdl::ZPosition DungeonSingleStairsDownTest::kStairsDownPosition = mdl::ZPosition(2, 1);


        class DungeonSingleStairsUpTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kStairsUpPosition;

            void SetUp() {
                mDungeon = new mdl::ZDungeon(4, 3, ""
                    "####"
                    "#.<#"
                    "####");
            }

            void TearDown() {
                delete mDungeon;
            }

            mdl::ZDungeon* mDungeon;
        };

        const mdl::ZPosition DungeonSingleStairsUpTest::kStairsUpPosition = mdl::ZPosition(2, 1);


        class DungeonTwoDifferentStairsTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kStairsUpPosition;
            const static mdl::ZPosition kStairsDownPosition;

            void SetUp() {
                mDungeon = new mdl::ZDungeon(4, 3, ""
                    "####"
                    "#><#"
                    "####");
            }

            void TearDown() {
                delete mDungeon;
            }

            mdl::ZDungeon* mDungeon;
        };

        const mdl::ZPosition DungeonTwoDifferentStairsTest::kStairsUpPosition = mdl::ZPosition(2, 1);
        const mdl::ZPosition DungeonTwoDifferentStairsTest::kStairsDownPosition = mdl::ZPosition(1, 1);


        class DungeonTwoStairsPairsTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kStairsUpPosition1st;
            const static mdl::ZPosition kStairsUpPosition2nd;
            const static mdl::ZPosition kStairsDownPosition1st;
            const static mdl::ZPosition kStairsDownPosition2nd;

            void SetUp() {
                mDungeon = new mdl::ZDungeon(6, 4, ""
                    "######"
                    "#<..>#"
                    "#.><.#"
                    "######");
            }

            void TearDown() {
                delete mDungeon;
            }

            mdl::ZDungeon* mDungeon;
        };

        const mdl::ZPosition DungeonTwoStairsPairsTest::kStairsUpPosition1st = mdl::ZPosition(1, 1);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kStairsUpPosition2nd = mdl::ZPosition(3, 2);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kStairsDownPosition1st = mdl::ZPosition(4, 1);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kStairsDownPosition2nd = mdl::ZPosition(2, 2);

        TEST_F(DungeonSingleStairsDownTest, GetStairsUp_ReturnsNoCellsForMapWithoutStairsUp) {
            ASSERT_EQ(0, mDungeon->GetStairsUp().size());
        }

        TEST_F(DungeonSingleStairsUpTest, GetStairsUp_ReturnsExactCellForMapWithSingleStairsUp) {
            ASSERT_EQ(1, mDungeon->GetStairsUp().size());
            ASSERT_CONTAINS(mDungeon->GetStairsUp(), kStairsUpPosition);
        }

        TEST_F(DungeonTwoDifferentStairsTest, GetStairsUp_ReturnsExactCellForMapWithTwoDifferentStairs) {
            ASSERT_EQ(1, mDungeon->GetStairsUp().size());
            ASSERT_CONTAINS(mDungeon->GetStairsUp(), kStairsUpPosition);
        }

        TEST_F(DungeonTwoStairsPairsTest, GetStairsUp_ReturnsTwoCellsForMapWithTwoStairsUp) {
            const mdl::ZDungeon::StairsList& stairs = mDungeon->GetStairsUp();

            EXPECT_EQ(2, stairs.size());
            EXPECT_CONTAINS(stairs, kStairsUpPosition1st);
            ASSERT_CONTAINS(stairs, kStairsUpPosition2nd);
        }

        TEST_F(DungeonSingleStairsUpTest, GetStairsDown_ReturnsNoCellsForMapWithoutStairsDown) {
            ASSERT_EQ(0, mDungeon->GetStairsDown().size());
        }

        TEST_F(DungeonSingleStairsDownTest, GetStairsDown_ReturnsExactCellForMapWithSingleStairsDown) {
            ASSERT_EQ(1, mDungeon->GetStairsDown().size());
            ASSERT_CONTAINS(mDungeon->GetStairsDown(), kStairsDownPosition);
        }

        TEST_F(DungeonTwoDifferentStairsTest, GetStairsDown_ReturnsExactCellForMapWithTwoDifferentStairs) {
            ASSERT_EQ(1, mDungeon->GetStairsDown().size());
            ASSERT_CONTAINS(mDungeon->GetStairsDown(), kStairsDownPosition);
        }

        TEST_F(DungeonTwoStairsPairsTest, GetStairsDown_ReturnsTwoCellsForMapWithTwoStairsDown) {
            const mdl::ZDungeon::StairsList& stairs = mDungeon->GetStairsDown();

            EXPECT_EQ(2, stairs.size());
            EXPECT_CONTAINS(stairs, kStairsDownPosition1st);
            ASSERT_CONTAINS(stairs, kStairsDownPosition2nd);
        }
    }
}
