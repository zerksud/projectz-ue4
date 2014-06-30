#include "przTestPrivatePCH.h"

#include "model/DungeonLevel.h"
#include "utils/Services.h"
#include "utils/UniqueIdRegistry.h"

#include "model/PositionTest.h"
#include "TestHelpers.h"

#define ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeonLevel) \
    EXPECT_EQ(kFailSafeMapWidth, (dungeonLevel).GetWidth()); \
    EXPECT_EQ(kFailSafeMapHeight, (dungeonLevel).GetHeight()); \
    ASSERT_TRUE((dungeonLevel).CellIsEmpty(kFailSafeMapEmptyCell));

namespace prz {
    namespace testing {
        class DungeonLevelTest : public ::testing::Test {
        protected:
            static const mdl::ZDungeonLevel::ZMapCell* kSomeMap;
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

        const mdl::ZDungeonLevel::ZMapCell* DungeonLevelTest::kSomeMap = ""
            "#####"
            "#<.>#"
            "#####";
        const mdl::ZPosition DungeonLevelTest::kSomeHollowCell = mdl::ZPosition(2, 1);
        const mdl::ZPosition DungeonLevelTest::kSomeSolidCell = mdl::ZPosition(1, 2);
        const mdl::ZPosition DungeonLevelTest::kUpStaircaseCell = mdl::ZPosition(1, 1);
        const mdl::ZPosition DungeonLevelTest::kDownStaircaseCell = mdl::ZPosition(3, 1);

        const mdl::ZPosition DungeonLevelTest::kFailSafeMapEmptyCell = mdl::ZPosition(1, 1);

        class DungeonLevelWithSomeMapTest : public DungeonLevelTest {
        protected:
            void SetUp() {
                mDungeonLevel = new mdl::ZDungeonLevel(kSomeMapWidth, kSomeMapHeight, kSomeMap);
            }

            void TearDown() {
                delete mDungeonLevel;
            }

            mdl::ZDungeonLevel* mDungeonLevel;
        };

        TEST_F(DungeonLevelWithSomeMapTest, GetWidthAndGetHeightReturnExpectedValues) {
            EXPECT_EQ(kSomeMapWidth, mDungeonLevel->GetWidth());
            ASSERT_EQ(kSomeMapHeight, mDungeonLevel->GetHeight());
        }

        TEST_F(DungeonLevelTest, IncorrectWidthCausesCreationOfDungeonWithFailSafeMap) {
            mdl::ZDungeonLevel dungeonLevel(kSomeIncorrectMapWidth, kSomeMapHeight, kSomeMap);

            ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeonLevel);
        }

        TEST_F(DungeonLevelTest, IncorrectHeightCausesCreationOfDungeonWithFailSafeMap) {
            mdl::ZDungeonLevel dungeonLevel(kSomeMapWidth, kSomeIncorrectMapHeight, kSomeMap);

            ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeonLevel);
        }

        TEST_F(DungeonLevelTest, MapWithSizeInconsistentWithWidthAndHeightCausesCreationOfDungeonWithFailSafeMap) {
            mdl::ZDungeonLevel dungeonLevel(kSomeMapWidth, kSomeMapHeight + 1, kSomeMap);

            ASSERT_DUNGEON_WITH_FAIL_SAFE_MAP(dungeonLevel);
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsSolid_HollowCellsAreNotSolid) {
            ASSERT_FALSE(mDungeonLevel->CellIsSolid(kSomeHollowCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsSolid_UpStaircaseCellsAreNotSolid) {
            ASSERT_FALSE(mDungeonLevel->CellIsSolid(kUpStaircaseCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsSolid_DownStaircaseCellsAreNotSolid) {
            ASSERT_FALSE(mDungeonLevel->CellIsSolid(kDownStaircaseCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsSolid_SolidCellsAreIndeedSolid) {
            ASSERT_TRUE(mDungeonLevel->CellIsSolid(kSomeSolidCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsSolid_CoordinatesWorksJustLikePosition) {
            ASSERT_TRUE(mDungeonLevel->CellIsSolid(kSomeSolidCell.GetX(), kSomeSolidCell.GetY()));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsEmpty_HollowCellsAreEmpty) {
            ASSERT_TRUE(mDungeonLevel->CellIsEmpty(kSomeHollowCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsEmpty_UpStaircaseCellsAreEmpty) {
            ASSERT_TRUE(mDungeonLevel->CellIsEmpty(kUpStaircaseCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsEmpty_DownStaircaseCellsAreEmpty) {
            ASSERT_TRUE(mDungeonLevel->CellIsEmpty(kDownStaircaseCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsEmpty_SolidCellsAreNotEmpty) {
            ASSERT_FALSE(mDungeonLevel->CellIsEmpty(kSomeSolidCell));
        }

        TEST_F(DungeonLevelWithSomeMapTest, CellIsEmpty_CoordinatesWorksJustLikePosition) {
            ASSERT_TRUE(mDungeonLevel->CellIsEmpty(kSomeHollowCell.GetX(), kSomeHollowCell.GetY()));
        }

        class DungeonSingleDownStaircaseTest : public ::testing::Test {
        protected:
            static const mdl::ZPosition kDownStaircasePosition;

            void SetUp() {
                mDungeonLevel = new mdl::ZDungeonLevel(4, 3, ""
                "####"
                "#.>#"
                "####");
            }

            void TearDown() {
                delete mDungeonLevel;
            }

            mdl::ZDungeonLevel* mDungeonLevel;
        };

        const mdl::ZPosition DungeonSingleDownStaircaseTest::kDownStaircasePosition = mdl::ZPosition(2, 1);


        class DungeonSingleUpStaircaseTest : public ::testing::Test {
        protected:
            static const mdl::ZPosition kUpStaircasePosition;

            void SetUp() {
                mDungeonLevel = new mdl::ZDungeonLevel(4, 3, ""
                    "####"
                    "#.<#"
                    "####");
            }

            void TearDown() {
                delete mDungeonLevel;
            }

            mdl::ZDungeonLevel* mDungeonLevel;
        };

        const mdl::ZPosition DungeonSingleUpStaircaseTest::kUpStaircasePosition = mdl::ZPosition(2, 1);


        class DungeonTwoDifferentStairsTest : public ::testing::Test {
        protected:
            static const mdl::ZPosition kUpStaircasePosition;
            static const mdl::ZPosition kDownStaircasePosition;

            void SetUp() {
                mDungeonLevel = new mdl::ZDungeonLevel(4, 3, ""
                    "####"
                    "#><#"
                    "####");
            }

            void TearDown() {
                delete mDungeonLevel;
            }

            mdl::ZDungeonLevel* mDungeonLevel;
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
                mDungeonLevel = new mdl::ZDungeonLevel(6, 4, ""
                    "######"
                    "#<..>#"
                    "#.><.#"
                    "######");
            }

            void TearDown() {
                delete mDungeonLevel;
            }

            mdl::ZDungeonLevel* mDungeonLevel;
        };

        const mdl::ZPosition DungeonTwoStairsPairsTest::kUpStaircasePosition1st = mdl::ZPosition(1, 1);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kUpStaircasePosition2nd = mdl::ZPosition(3, 2);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kDownStaircasePosition1st = mdl::ZPosition(4, 1);
        const mdl::ZPosition DungeonTwoStairsPairsTest::kDownStaircasePosition2nd = mdl::ZPosition(2, 2);

        TEST_F(DungeonSingleDownStaircaseTest, GetUpStaircase_ReturnsNoCellsForMapWithoutUpStaircase) {
            ASSERT_EQ(0, mDungeonLevel->GetUpStaircases().size());
        }

        TEST_F(DungeonSingleUpStaircaseTest, GetUpStaircase_ReturnsExactCellForMapWithSingleUpStaircase) {
            ASSERT_EQ(1, mDungeonLevel->GetUpStaircases().size());
            ASSERT_CONTAINS(mDungeonLevel->GetUpStaircases(), kUpStaircasePosition);
        }

        TEST_F(DungeonTwoDifferentStairsTest, GetUpStaircase_ReturnsExactCellForMapWithTwoDifferentStairs) {
            ASSERT_EQ(1, mDungeonLevel->GetUpStaircases().size());
            ASSERT_CONTAINS(mDungeonLevel->GetUpStaircases(), kUpStaircasePosition);
        }

        TEST_F(DungeonTwoStairsPairsTest, GetUpStaircase_ReturnsTwoCellsForMapWithTwoUpStaircase) {
            const mdl::ZDungeonLevel::StaircaseList& stairs = mDungeonLevel->GetUpStaircases();

            EXPECT_EQ(2, stairs.size());
            EXPECT_CONTAINS(stairs, kUpStaircasePosition1st);
            ASSERT_CONTAINS(stairs, kUpStaircasePosition2nd);
        }

        TEST_F(DungeonSingleUpStaircaseTest, GetDownStaircase_ReturnsNoCellsForMapWithoutDownStaircase) {
            ASSERT_EQ(0, mDungeonLevel->GetDownStaircases().size());
        }

        TEST_F(DungeonSingleDownStaircaseTest, GetDownStaircase_ReturnsExactCellForMapWithSingleDownStaircase) {
            ASSERT_EQ(1, mDungeonLevel->GetDownStaircases().size());
            ASSERT_CONTAINS(mDungeonLevel->GetDownStaircases(), kDownStaircasePosition);
        }

        TEST_F(DungeonTwoDifferentStairsTest, GetDownStaircase_ReturnsExactCellForMapWithTwoDifferentStairs) {
            ASSERT_EQ(1, mDungeonLevel->GetDownStaircases().size());
            ASSERT_CONTAINS(mDungeonLevel->GetDownStaircases(), kDownStaircasePosition);
        }

        TEST_F(DungeonTwoStairsPairsTest, GetDownStaircase_ReturnsTwoCellsForMapWithTwoDownStaircase) {
            const mdl::ZDungeonLevel::StaircaseList& stairs = mDungeonLevel->GetDownStaircases();

            EXPECT_EQ(2, stairs.size());
            EXPECT_CONTAINS(stairs, kDownStaircasePosition1st);
            ASSERT_CONTAINS(stairs, kDownStaircasePosition2nd);
        }

        class DungeonMonsterTest : public DungeonLevelWithSomeMapTest {
        protected:
            void SetUp() {
                using namespace utl;

                DungeonLevelWithSomeMapTest::SetUp();

                REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new ZUniqueIdRegistry());
            }

            void TearDown() {
                using namespace utl;

                DungeonLevelWithSomeMapTest::TearDown();

                UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);
            }
        };

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCanBePlacedInEmptyCell) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            ASSERT_TRUE(mDungeonLevel->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCanBePlacedInStaircaseCell) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kUpStaircaseCell;

            ASSERT_TRUE(mDungeonLevel->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_NonRegisteredMonstersAreNotPermitted) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            monster.SetId(utl::ZRegistrable::kNoId);
            ZPosition validPosition = kSomeHollowCell;

            ASSERT_FALSE(mDungeonLevel->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCantBePlacedInSolidCell) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeSolidCell;

            ASSERT_FALSE(mDungeonLevel->PlaceMonster(monster, validPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_SameMonsterCantBePlacedTwice) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kUpStaircaseCell;
            ZPosition anotherValidPosition = kUpStaircaseCell;

            mDungeonLevel->PlaceMonster(monster, validPosition);
            ASSERT_FALSE(mDungeonLevel->PlaceMonster(monster, anotherValidPosition));
        }

        TEST_F(DungeonMonsterTest, PlaceMonster_MonsterCantBePlacedInCellWhichIsOccupiedByAnotherMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            mDungeonLevel->PlaceMonster(monster, validPosition);

            ZMonster anotherMonster = ZMonster::CreateMonster();
            ASSERT_FALSE(mDungeonLevel->PlaceMonster(anotherMonster, validPosition));
        }

        TEST_F(DungeonMonsterTest, CellIsEmpty_OccupiedCellsAreNotEmpty) {
            mDungeonLevel->PlaceMonster(mdl::ZMonster::CreateMonster(), kSomeHollowCell);
            ASSERT_FALSE(mDungeonLevel->CellIsEmpty(kSomeHollowCell));
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsPositionOfPlacedMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            mDungeonLevel->PlaceMonster(monster, validPosition);

            ZPosition returnedMonsterPosition = *mDungeonLevel->GetMonsterPosition(monster.GetId());

            ASSERT_POSITION_EQ(validPosition, returnedMonsterPosition);
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsExactMonsterPositionInsteadOfSomeMonsterPosition) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            ZPosition validPosition = kSomeHollowCell;

            ZMonster anotherMonster = ZMonster::CreateMonster();
            ZPosition anotherValidPosition = kUpStaircaseCell;

            mDungeonLevel->PlaceMonster(monster, validPosition);
            mDungeonLevel->PlaceMonster(anotherMonster, anotherValidPosition);

            ZPosition returnedMonsterPosition = *mDungeonLevel->GetMonsterPosition(anotherMonster.GetId());

            ASSERT_POSITION_EQ(anotherValidPosition, returnedMonsterPosition);
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsNullptrForNotPlacedMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();

            ASSERT_EQ(nullptr, mDungeonLevel->GetMonsterPosition(monster.GetId()));
        }

        TEST_F(DungeonMonsterTest, GetMonster_ReturnsPlacedMonster) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();
            monster.GetDirection().Turn(ETurnDirection::Back);
            ZPositionDiff monsterPredictedMove = monster.GetDirection().PredictMove();

            ZPosition validPosition = kSomeHollowCell;

            mDungeonLevel->PlaceMonster(monster, validPosition);

            ZPositionDiff returnedMonsterPredictedMove = mDungeonLevel->GetMonster(monster.GetId())->GetDirection().PredictMove();

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

            mDungeonLevel->PlaceMonster(monster, validPosition);
            mDungeonLevel->PlaceMonster(anotherMonster, anotherValidPosition);

            ZPositionDiff returnedMonsterPredictedMove = mDungeonLevel->GetMonster(anotherMonster.GetId())->GetDirection().PredictMove();

            ASSERT_POSITION_DIFF_EQ(monsterPredictedMove, returnedMonsterPredictedMove);
        }

        TEST_F(DungeonMonsterTest, GetMonsterPosition_ReturnsNullptrForNotPlacedMonsterId) {
            using namespace mdl;

            ZMonster monster = ZMonster::CreateMonster();

            ASSERT_EQ(nullptr, mDungeonLevel->GetMonster(monster.GetId()));
        }

        TEST_F(DungeonLevelTest, Destructor_ReleasesMonsterIds) {
            using namespace mdl;
            using namespace utl;

            ZDungeonLevel* dungeonLevel = new ZDungeonLevel(kSomeMapWidth, kSomeMapHeight, kSomeMap);

            REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new ZUniqueIdRegistry());
            ZMonster monster = ZMonster::CreateMonster();
            ZIdType monsterId = monster.GetId();

            dungeonLevel->PlaceMonster(monster, kSomeHollowCell);
            delete dungeonLevel;

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

                mDungeonLevel = new ZDungeonLevel(kMapWidth, kMapHeight, kMap);

                ZMonster monster = ZMonster::CreateMonster();
                monster.GetDirection().Turn(ETurnDirection::Left);
                mMonsterId = monster.GetId();
                mDungeonLevel->PlaceMonster(monster, kDeadEndPosition);

                monster = ZMonster::CreateMonster();
                mAnotherMonsterId = monster.GetId();
                mDungeonLevel->PlaceMonster(monster, kAnotherMonsterPosition);
            }

            void TearDown() {
                using namespace utl;

                delete mDungeonLevel;
                UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);
            }

            mdl::ZDungeonLevel* mDungeonLevel;
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
            ASSERT_TRUE(mDungeonLevel->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward));
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnsCorrectExpectedMoveDiffForMoveIntoEmptyCell) {
            mdl::ZPositionDiff expectedMoveDiff;
            mDungeonLevel->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward, &expectedMoveDiff);

            ASSERT_POSITION_DIFF_EQ(kExpectedForwardMoveDiff, expectedMoveDiff);
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnsFalseForMoveIntoSolidCell) {
            ASSERT_FALSE(mDungeonLevel->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Right));
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnsCorrectExpectedMoveDiffForMoveIntoSolidCell) {
            mdl::ZPositionDiff expectedMoveDiff;
            mDungeonLevel->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Right, &expectedMoveDiff);

            ASSERT_POSITION_DIFF_EQ(kExpectedRightMoveDiff, expectedMoveDiff);
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_DiagonalMovesAroundCornerAreNotPermitted) {
            mDungeonLevel->GetMonster(mMonsterId)->GetDirection().Turn(mdl::ETurnDirection::ForwardRight);

            ASSERT_FALSE(mDungeonLevel->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward));
        }

        TEST_F(DungeonMoveMonsterTest, TryToMoveMonster_ReturnFalseForMoveIntoOccupiedCell) {
            mDungeonLevel->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward);

            ASSERT_FALSE(mDungeonLevel->TryToMoveMonster(mMonsterId, mdl::EMoveDirection::Forward));
        }

    }
}
