#include "przTestPrivatePCH.h"

#include "model/Dungeon.h"
#include "utils/Services.h"
#include "utils/UniqueIdRegistry.h"

#include "model/PositionTest.h"
#include "TestHelpers.h"

#define ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeon) \
    EXPECT_EQ(kFailSafeMapWidth, (dungeon).GetWidth()); \
    EXPECT_EQ(kFailSafeMapHeight, (dungeon).GetHeight()); \
    ASSERT_TRUE((dungeon).CellIsEmpty(kFailSafeMapEmptyCell));

namespace prz {
    namespace testing {
        class DungeonTest : public ::testing::Test {
        protected:
            static const mdl::ZDungeon::ZMapCell* kSomeMap;
            static const int kSomeMapWidth = 5;
            static const int kSomeMapHeight = 3;
            static const mdl::ZPosition kSomeHollowCell;
            static const mdl::ZPosition kSomeSolidCell;
            static const mdl::ZPosition kUpStaircaseCell;
            static const mdl::ZPosition kDownStaircaseCell;

            static const int kSomeIncorrectMapWidth = 0;
            static const int kSomeIncorrectMapHeight = -5;

            static const int kFailSafeMapWidth = 3;
            static const int kFailSafeMapHeight = 3;
            static const mdl::ZPosition kFailSafeMapEmptyCell;
        };

        const mdl::ZDungeon::ZMapCell* DungeonTest::kSomeMap = ""
            "#####"
            "#<.>#"
            "#####";
        const mdl::ZPosition DungeonTest::kSomeHollowCell = mdl::ZPosition(2, 1);
        const mdl::ZPosition DungeonTest::kSomeSolidCell = mdl::ZPosition(1, 2);
        const mdl::ZPosition DungeonTest::kUpStaircaseCell = mdl::ZPosition(1, 1);
        const mdl::ZPosition DungeonTest::kDownStaircaseCell = mdl::ZPosition(3, 1);

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
            ASSERT_FALSE(mDungeon->CellIsSolid(kSomeHollowCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_UpStaircaseCellsAreNotSolid) {
            ASSERT_FALSE(mDungeon->CellIsSolid(kUpStaircaseCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_DownStaircaseCellsAreNotSolid) {
            ASSERT_FALSE(mDungeon->CellIsSolid(kDownStaircaseCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_SolidCellsAreIndeedSolid) {
            ASSERT_TRUE(mDungeon->CellIsSolid(kSomeSolidCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsSolid_CoordinatesWorksJustLikePosition) {
            ASSERT_TRUE(mDungeon->CellIsSolid(kSomeSolidCell.GetX(), kSomeSolidCell.GetY()));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_HollowCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeHollowCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_UpStaircaseCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kUpStaircaseCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_DownStaircaseCellsAreEmpty) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kDownStaircaseCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_SolidCellsAreNotEmpty) {
            ASSERT_FALSE(mDungeon->CellIsEmpty(kSomeSolidCell));
        }

        TEST_F(DungeonWithSomeMapTest, CellIsEmpty_CoordinatesWorksJustLikePosition) {
            ASSERT_TRUE(mDungeon->CellIsEmpty(kSomeHollowCell.GetX(), kSomeHollowCell.GetY()));
        }

        class DungeonSingleDownStaircaseTest : public ::testing::Test {
        protected:
            static const mdl::ZPosition kDownStaircasePosition;

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
            static const mdl::ZPosition kUpStaircasePosition;

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
            static const mdl::ZPosition kUpStaircasePosition;
            static const mdl::ZPosition kDownStaircasePosition;

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
            static const mdl::ZPosition kUpStaircasePosition1st;
            static const mdl::ZPosition kUpStaircasePosition2nd;
            static const mdl::ZPosition kDownStaircasePosition1st;
            static const mdl::ZPosition kDownStaircasePosition2nd;

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

        class DungeonMonsterTest : public DungeonWithSomeMapTest {
        protected:
            void SetUp() {
                using namespace utl;

                DungeonWithSomeMapTest::SetUp();

                REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new ZUniqueIdRegistry());
            }

            void TearDown() {
                using namespace utl;

                DungeonWithSomeMapTest::TearDown();

                UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);
            }
        };

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCanBePlacedInEmptyCell) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            ASSERT_TRUE(mDungeon->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCanBePlacedInStaircaseCell) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kUpStaircaseCell;

            ASSERT_TRUE(mDungeon->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_NonRegisteredMonstersAreNotPermitted) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            monster.SetId(utl::ZRegistrable::kNoId);
            ZPosition validPosition = kSomeHollowCell;

            ASSERT_FALSE(mDungeon->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCantBePlacedInSolidCell) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeSolidCell;

            ASSERT_FALSE(mDungeon->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_SameMonsterCantBePlacedTwice) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kUpStaircaseCell;
            ZPosition anotherValidPosition = kUpStaircaseCell;

            mDungeon->PlaceMonster(monster, validPosition);
            ASSERT_FALSE(mDungeon->PlaceMonster(monster, anotherValidPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCantBePlacedInCellWhichIsOccupiedByAnotherMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            mDungeon->PlaceMonster(monster, validPosition);

            ZMonster anotherMonster = ZMonster::CreateMonster();
            ASSERT_FALSE(mDungeon->PlaceMonster(anotherMonster, validPosition));
        }

        TEST_F(DungeonMonsterTest, CellIsEmpty_OccupiedCellsAreNotEmpty) {
            mDungeon->PlaceMonster(mdl::ZMonster::CreateMonster(), kSomeHollowCell);
            ASSERT_FALSE(mDungeon->CellIsEmpty(kSomeHollowCell));
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsPositionOfPlacedMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            mDungeon->PlaceMonster(monster, validPosition);

            ZPosition returnedMonsterPosition = *mDungeon->GetMonsterPosition(monster.GetId());

            ASSERT_POSITION_EQ(validPosition, returnedMonsterPosition);
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsExactMonsterPositionInsteadOfSomeMonsterPosition) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            ZMonster anotherMonster = ZMonster::CreateMonster();
            ZPosition anotherValidPosition = kUpStaircaseCell;

            mDungeon->PlaceMonster(monster, validPosition);
            mDungeon->PlaceMonster(anotherMonster, anotherValidPosition);

            ZPosition returnedMonsterPosition = *mDungeon->GetMonsterPosition(anotherMonster.GetId());

            ASSERT_POSITION_EQ(anotherValidPosition, returnedMonsterPosition);
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsNullptrForNotPlacedMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();

            ASSERT_EQ(nullptr, mDungeon->GetMonsterPosition(monster.GetId()));
        }

        TEST_F(DungeonMonsterTest, GetMonster_ReturnsPlacedMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            monster.GetDirection().Turn(ETurnDirection::Back);
            ZPositionDiff monsterPredictedMove = monster.GetDirection().PredictMove();

            ZPosition validPosition = kSomeHollowCell;

            mDungeon->PlaceMonster(monster, validPosition);

            ZPositionDiff returnedMonsterPredictedMove = mDungeon->GetMonster(monster.GetId())->GetDirection().PredictMove();

            ASSERT_POSITION_DIFF_EQ(monsterPredictedMove, returnedMonsterPredictedMove);
        }

        TEST_F(DungeonMonsterTest, GetMonster_ReturnsExactlyPlacedMonsterInsteadOfSomeOtherMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            ZMonster anotherMonster = ZMonster::CreateMonster();
            anotherMonster.GetDirection().Turn(ETurnDirection::Back);
            ZPositionDiff monsterPredictedMove = anotherMonster.GetDirection().PredictMove();
            ZPosition anotherValidPosition = kUpStaircaseCell;

            mDungeon->PlaceMonster(monster, validPosition);
            mDungeon->PlaceMonster(anotherMonster, anotherValidPosition);

            ZPositionDiff returnedMonsterPredictedMove = mDungeon->GetMonster(anotherMonster.GetId())->GetDirection().PredictMove();

            ASSERT_POSITION_DIFF_EQ(monsterPredictedMove, returnedMonsterPredictedMove);
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsNullptrForNotPlacedMonsterId) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();

            ASSERT_EQ(nullptr, mDungeon->GetMonster(monster.GetId()));
        }

        TEST_F(DungeonTest, Destructor_ReleasesMonsterIds) {
            using namespace mdl;
            using namespace utl;

            ZDungeon* dungeon = new ZDungeon(kSomeMapWidth, kSomeMapHeight, kSomeMap);

            REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new ZUniqueIdRegistry());
            ZMonster monster = ZMonster::CreateMonster();
            ZIdType monsterId = monster.GetId();

            dungeon->PlaceMonster(monster, kSomeHollowCell);
            delete dungeon;

            ZMonster anotherMonster = ZMonster::CreateMonster();

            UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);

            ASSERT_EQ(monsterId, anotherMonster.GetId());
        }

        class DungeonMoveMonsterTest : public ::testing::Test {
        protected:
            static const int kMapWidth = 5;
            static const int kMapHeight = 5;
            static const char* kMap;

            static const mdl::ZPosition kDeadEndPosition;
            static const mdl::ZPositionDiff kExpectedForwardMoveDiff;
            static const mdl::ZPositionDiff kExpectedRightMoveDiff;
            static const mdl::ZPosition kAnotherMonsterPosition;

            void SetUp() {
                using namespace mdl;
                using namespace utl;

                REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new ZUniqueIdRegistry());

                mDungeon = new ZDungeon(kMapWidth, kMapHeight, kMap);

                ZMonster monster = ZMonster::CreateMonster();
                monster.GetDirection().Turn(ETurnDirection::Left);
                mMonsterId = monster.GetId();
                mDungeon->PlaceMonster(monster, kDeadEndPosition);

                monster = ZMonster::CreateMonster();
                mAnotherMonsterId = monster.GetId();
                mDungeon->PlaceMonster(monster, kAnotherMonsterPosition);
            }

            void TearDown() {
                using namespace utl;

                delete mDungeon;
                UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);
            }

            mdl::ZDungeon* mDungeon;
            utl::ZIdType mMonsterId;
            utl::ZIdType mAnotherMonsterId;
        };

        const char* DungeonMoveMonsterTest::kMap = ""
            "#####"
            "#...#"
            "#...#"
            "##.##"
            "#####";
        const mdl::ZPosition DungeonMoveMonsterTest::kDeadEndPosition = mdl::ZPosition(2, 3);
        const mdl::ZPositionDiff DungeonMoveMonsterTest::kExpectedForwardMoveDiff = mdl::ZPositionDiff(0, -1);
        const mdl::ZPositionDiff DungeonMoveMonsterTest::kExpectedRightMoveDiff = mdl::ZPositionDiff(1, 0);
        const mdl::ZPosition DungeonMoveMonsterTest::kAnotherMonsterPosition = mdl::ZPosition(2, 1);

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnsTrueForMoveIntoEmptyCell) {
            ASSERT_TRUE(mDungeon->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward));
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnsCorrectExpectedMoveDiffForMoveIntoEmptyCell) {
            mdl::ZPositionDiff expectedMoveDiff;
            mDungeon->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward, &expectedMoveDiff);

            ASSERT_POSITION_DIFF_EQ(kExpectedForwardMoveDiff, expectedMoveDiff);
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnsFalseForMoveIntoSolidCell) {
            ASSERT_FALSE(mDungeon->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Right));
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnsCorrectExpectedMoveDiffForMoveIntoSolidCell) {
            mdl::ZPositionDiff expectedMoveDiff;
            mDungeon->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Right, &expectedMoveDiff);

            ASSERT_POSITION_DIFF_EQ(kExpectedRightMoveDiff, expectedMoveDiff);
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_DiagonalMovesAroundCornerAreNotPermitted) {
            mDungeon->GetMonster(mMonsterId)->GetDirection().Turn(mdl::ETurnDirection::ForwardRight);

            ASSERT_FALSE(mDungeon->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward));
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnFalseForMoveIntoOccupiedCell) {
            mDungeon->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward);

            ASSERT_FALSE(mDungeon->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward));
        }

    }
}
