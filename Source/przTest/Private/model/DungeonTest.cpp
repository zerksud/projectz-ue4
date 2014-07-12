#include "przTestPrivatePCH.h"

#include "model/Dungeon.h"

#include <functional>

#include "model/IDungeonLevelGenerator.h"
#include "utils/Services.h"
#include "utils/UniqueIdRegistry.h"

#include "model/PositionTest.h"

namespace prz {
    namespace testing {
        class TestLevelGenerator : public mdl::IDungeonLevelGenerator {
        public:
            typedef std::function<mdl::ZDungeonLevel* ()> SimpleLevelGenerator;

            TestLevelGenerator(SimpleLevelGenerator generator) :
                mSimpleLevelGenerator(generator),
                mGenerateLevelCalls(0) {
            }

            virtual mdl::ZDungeonLevel* GenerateLevel(const mdl::ZDungeonLevel::StaircaseList& upStaircases) {
                mLastUpStaircases = upStaircases;
                ++mGenerateLevelCalls;
                return mSimpleLevelGenerator();
            }

            const mdl::ZDungeonLevel::StaircaseList& GetLastUpStaircases() const {
                return mLastUpStaircases;
            }

            int GetGenerateLevelCalls() const {
                return mGenerateLevelCalls;
            }

        private:
            SimpleLevelGenerator mSimpleLevelGenerator;
            mdl::ZDungeonLevel::StaircaseList mLastUpStaircases;
            int mGenerateLevelCalls;
        };

        class DungeonTest : public ::testing::Test {
        protected:
            static const size_t kLevelWidth = 4;
            static const size_t kLevelHeight = 3;
            static const mdl::ZDungeonLevel::ZMapCell* kLevelMap;
            static mdl::ZPosition kDownStaircasePosition;
            static mdl::ZPosition kUpStaircasePosition;

            virtual void SetUp() override {
                using namespace mdl;

                REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new utl::ZUniqueIdRegistry());

                mGenerator = new TestLevelGenerator([]() {
                    return new ZDungeonLevel(kLevelWidth, kLevelHeight, kLevelMap);
                });
                mDungeon = new ZDungeon(mGenerator);
            }

            virtual void TearDown() override {
                UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);
                delete mDungeon;
            }

            mdl::ZDungeon* mDungeon;
            TestLevelGenerator* mGenerator;
        };

        const mdl::ZDungeonLevel::ZMapCell* DungeonTest::kLevelMap =
            "####"
            "#><#"
            "####";

        mdl::ZPosition DungeonTest::kDownStaircasePosition = mdl::ZPosition(1, 1);
        mdl::ZPosition DungeonTest::kUpStaircasePosition = mdl::ZPosition(2, 1);

        TEST_F(DungeonTest, GetLevel_GeneratesNewLevelsOnRequest) {
            mDungeon->GetLevel(0);
            ASSERT_EQ(1, mGenerator->GetGenerateLevelCalls());
        }

        TEST_F(DungeonTest, LevelGeneration_UsesLevelGeneratorPassedInConstructor) {
            using namespace mdl;
            ZDungeonLevel* level = new ZDungeonLevel(kLevelWidth, kLevelHeight, kLevelMap);
            TestLevelGenerator* generator = new TestLevelGenerator([level]() {
                return level;
            });

            ZDungeon dungeon(generator);

            ASSERT_EQ(level, dungeon.GetLevel(0));
        }

        TEST_F(DungeonTest, LevelGeneration_UsesDownStaircasesFromPreviousLevelToGenerateNextOne) {
            mDungeon->GetLevel(0);
            mDungeon->GetLevel(1);

            ASSERT_POSITION_EQ(kDownStaircasePosition, mGenerator->GetLastUpStaircases()[0]);
        }

        TEST_F(DungeonTest, PlaceMonster_GeneratesNewLevelsOnRequest) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 10;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);

            ASSERT_EQ(levelIndex + 1, mGenerator->GetGenerateLevelCalls());
        }

        TEST_F(DungeonTest, PlaceMonster_ActuallyPlacesMonsterInAppropriateLevel) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);

            ASSERT_TRUE(mDungeon->GetLevel(levelIndex)->GetMonsterPosition(monster->GetId()) != nullptr);
        }

        TEST_F(DungeonTest, GetMonsterLevelIndex_ReturnsCorrectIndex) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);

            ASSERT_EQ(levelIndex, mDungeon->GetMonsterLevelIndex(monster->GetId()));
        }

        TEST_F(DungeonTest, GetMonsterLevelIndex_SetSuccessFlagToFalseOnFailure) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            bool successFlag = true;
            mDungeon->GetMonsterLevelIndex(monster->GetId(), &successFlag);

            ASSERT_FALSE(successFlag);
        }
        TEST_F(DungeonTest, GetMonsterLevelIndex_SetSuccessFlagToTrueOnActualSuccess) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            bool successFlag = false;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);
            mDungeon->GetMonsterLevelIndex(monster->GetId(), &successFlag);

            ASSERT_TRUE(successFlag);
        }

        TEST_F(DungeonTest, RemoveMonster_DoesntAllowRemovingOfNotPlacedMonsters) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();

            ASSERT_TRUE(mDungeon->RemoveMonster(monster->GetId()) == nullptr);
        }

        TEST_F(DungeonTest, RemoveMonster_ActuallyRemovesMonsterFromAppropriateLevel) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);
            mDungeon->RemoveMonster(monster->GetId());

            EXPECT_TRUE(mDungeon->GetLevel(levelIndex)->GetMonsterPosition(monster->GetId()) == nullptr);
            delete monster;
        }

        TEST_F(DungeonTest, TryToMoveMonster_ActuallyMovesMonsterInAppropriateLevel) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);
            mDungeon->TryToMoveMonster(monster->GetId(), mdl::EMoveDirection::Forward);

            ASSERT_POSITION_EQ(kUpStaircasePosition, *mDungeon->GetLevel(levelIndex)->GetMonsterPosition(monster->GetId()));
        }

        TEST_F(DungeonTest, TryToMoveMonster_MovesMonsterToAnotherLevelThroughStaircases) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);
            mDungeon->TryToMoveMonster(monster->GetId(), mdl::EMoveDirection::Down);

            ASSERT_TRUE(mDungeon->GetLevel(levelIndex + 1)->GetMonsterPosition(monster->GetId()) == nullptr);
        }

        TEST_F(DungeonTest, TryToMoveMonster_DoesntMoveMonsterToAnotherLevelFromCellWithoutAppropriateStaircase) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 5;
            mDungeon->PlaceMonster(monster, levelIndex, kDownStaircasePosition);
            mDungeon->TryToMoveMonster(monster->GetId(), mdl::EMoveDirection::Up);

            ASSERT_TRUE(mDungeon->GetLevel(levelIndex)->GetMonsterPosition(monster->GetId()) != nullptr);
        }

        TEST_F(DungeonTest, TryToMoveMonster_DoesntAllowMonsterToMoveUpFromFirstLevel) {
            mdl::ZMonster* monster = mdl::ZMonster::CreateMonster();
            const int levelIndex = 0;
            mDungeon->PlaceMonster(monster, levelIndex, kUpStaircasePosition);

            ASSERT_FALSE(mDungeon->TryToMoveMonster(monster->GetId(), mdl::EMoveDirection::Up));
        }

    }
}
