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
            const static mdl::ZPosition kSomeMapUpStaircaseCell;
            const static mdl::ZPosition kSomeMapDownStaircaseCell;

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
        const mdl::ZPosition DungeonTest::kSomeMapUpStaircaseCell = mdl::ZPosition(1, 1);
        const mdl::ZPosition DungeonTest::kSomeMapDownStaircaseCell = mdl::ZPosition(3, 1);

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

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_UpStaircaseCellsAreNotSolid) {
            ASSERT_FALSE(mDungeon->CellIsSolid(kSomeMapUpStaircaseCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_DownStaircaseCellsAreNotSolid) {
            ASSERT_FALSE(mDungeon->CellIsSolid(kSomeMapDownStaircaseCell));
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

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_UpStaircaseCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeMapUpStaircaseCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_DownStaircaseCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeMapDownStaircaseCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_SolidCellsAreNotEmpty) {
            ASSERT_FALSE(mDungeon->CellIsEmpty(kSomeMapSomeSolidCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_CoordinatesWorksJustLikePosition) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeMapSomeHollowCell.GetX(), kSomeMapSomeHollowCell.GetY()));
        }

        class DungeonSingleDownStaircaseTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kDownStaircasePosition;

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

        const mdl::ZPosition DungeonSingleDownStaircaseTest::kDownStaircasePosition = mdl::ZPosition(2, 1);


        class DungeonSingleUpStaircaseTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kUpStaircasePosition;

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

        const mdl::ZPosition DungeonSingleUpStaircaseTest::kUpStaircasePosition = mdl::ZPosition(2, 1);


        class DungeonTwoDifferentStairsTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kUpStaircasePosition;
            const static mdl::ZPosition kDownStaircasePosition;

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

        const mdl::ZPosition DungeonTwoDifferentStairsTest::kUpStaircasePosition = mdl::ZPosition(2, 1);
        const mdl::ZPosition DungeonTwoDifferentStairsTest::kDownStaircasePosition = mdl::ZPosition(1, 1);


        class DungeonTwoStairsPairsTest : public ::testing::Test {
        protected:
            const static mdl::ZPosition kUpStaircasePosition1st;
            const static mdl::ZPosition kUpStaircasePosition2nd;
            const static mdl::ZPosition kDownStaircasePosition1st;
            const static mdl::ZPosition kDownStaircasePosition2nd;

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

        const mdl::ZPosition DungeonTwoStairsPairsTest::kUpStaircasePosition1st = mdl::ZPosition(1, 1);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kUpStaircasePosition2nd = mdl::ZPosition(3, 2);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kDownStaircasePosition1st = mdl::ZPosition(4, 1);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kDownStaircasePosition2nd = mdl::ZPosition(2, 2);

        TEST_F(DungeonSingleDownStaircaseTest, GetUpStaircase_ReturnsNoCellsForMapWithoutUpStaircase) {
            ASSERT_EQ(0, mDungeon->GetUpStaircases().size());
        }

        TEST_F(DungeonSingleUpStaircaseTest, GetUpStaircase_ReturnsExactCellForMapWithSingleUpStaircase) {
            ASSERT_EQ(1, mDungeon->GetUpStaircases().size());
            ASSERT_CONTAINS(mDungeon->GetUpStaircases(), kUpStaircasePosition);
        }

        TEST_F(DungeonTwoDifferentStairsTest, GetUpStaircase_ReturnsExactCellForMapWithTwoDifferentStairs) {
            ASSERT_EQ(1, mDungeon->GetUpStaircases().size());
            ASSERT_CONTAINS(mDungeon->GetUpStaircases(), kUpStaircasePosition);
        }

        TEST_F(DungeonTwoStairsPairsTest, GetUpStaircase_ReturnsTwoCellsForMapWithTwoUpStaircase) {
            const mdl::ZDungeon::StaircaseList& stairs = mDungeon->GetUpStaircases();

            EXPECT_EQ(2, stairs.size());
            EXPECT_CONTAINS(stairs, kUpStaircasePosition1st);
            ASSERT_CONTAINS(stairs, kUpStaircasePosition2nd);
        }

        TEST_F(DungeonSingleUpStaircaseTest, GetDownStaircase_ReturnsNoCellsForMapWithoutDownStaircase) {
            ASSERT_EQ(0, mDungeon->GetDownStaircases().size());
        }

        TEST_F(DungeonSingleDownStaircaseTest, GetDownStaircase_ReturnsExactCellForMapWithSingleDownStaircase) {
            ASSERT_EQ(1, mDungeon->GetDownStaircases().size());
            ASSERT_CONTAINS(mDungeon->GetDownStaircases(), kDownStaircasePosition);
        }

        TEST_F(DungeonTwoDifferentStairsTest, GetDownStaircase_ReturnsExactCellForMapWithTwoDifferentStairs) {
            ASSERT_EQ(1, mDungeon->GetDownStaircases().size());
            ASSERT_CONTAINS(mDungeon->GetDownStaircases(), kDownStaircasePosition);
        }

        TEST_F(DungeonTwoStairsPairsTest, GetDownStaircase_ReturnsTwoCellsForMapWithTwoDownStaircase) {
            const mdl::ZDungeon::StaircaseList& stairs = mDungeon->GetDownStaircases();

            EXPECT_EQ(2, stairs.size());
            EXPECT_CONTAINS(stairs, kDownStaircasePosition1st);
            ASSERT_CONTAINS(stairs, kDownStaircasePosition2nd);
        }
    }
}
