#include "przTestPrivatePCH.h"

#include "model/Monster.h"
#include "utils/IUniqueIdRegistry.h"
#include "utils/Services.h"

#include "model/PositionTest.h"

namespace prz {
    namespace testing {

        class MonsterWithInstanceTest : public ::testing::Test {
        protected:
            static const mdl::ZPositionDiff kForwardPositionDiff;
            static const mdl::ZPositionDiff kBackwardPositionDiff;

            void SetUp() {
                mMonster = mdl::ZMonster::CreateMonster();
            }

            void TearDown() {
                delete mMonster;
            }

            mdl::ZMonster* mMonster;
        };

        const mdl::ZPositionDiff MonsterWithInstanceTest::kForwardPositionDiff = mdl::ZPositionDiff(1, 0);
        const mdl::ZPositionDiff MonsterWithInstanceTest::kBackwardPositionDiff = mdl::ZPositionDiff(-1, 0);


        TEST_F(MonsterWithInstanceTest, Constructor_DefaultMonsterDirectionIsForward) {
            mdl::ZPositionDiff expectedMove = mMonster->GetDirection().PredictMove();
            ASSERT_POSITION_DIFF_EQ(kForwardPositionDiff, expectedMove);
        }

        TEST_F(MonsterWithInstanceTest, HoldsDirectionBetweenCalls) {
            mdl::ZDirection& direction = mMonster->GetDirection();
            direction.Turn(mdl::ETurnDirection::Back);

            mdl::ZPositionDiff expectedMove = mMonster->GetDirection().PredictMove();
            ASSERT_POSITION_DIFF_EQ(kBackwardPositionDiff, expectedMove);
        }

        class UniqueIdRegistryMonsterTestMock : public utl::IUniqueIdRegistry {
        public:
            static const int kSomeId = 42;

            virtual bool AssignUniqueId(utl::ZRegistrable* object) override {
                object->SetId(kSomeId);
                return true;
            }
            virtual bool ReleaseUniqueId(utl::ZRegistrable* object) override {
                return true;
            }
        };

        TEST(MonsterTest, CreateMonster_CreatesUnregisteredMonsterIfThereIsNoIUniqueIdRegistryService) {
            using namespace utl;
            using namespace mdl;

            UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);
            ZMonster* monster = ZMonster::CreateMonster();
            ZIdType monsterId = monster->GetId();
            delete monster;

            ASSERT_EQ(ZRegistrable::kNoId, monsterId);
        }

        TEST(MonsterTest, CreateMonster_CreatesRegisteredMonsterIfThereIsIUniqueIdRegistryService) {
            using namespace utl;
            using namespace mdl;

            REGISTER_SERVICE(prz::utl::IUniqueIdRegistry, new UniqueIdRegistryMonsterTestMock());
            ZMonster* monster = ZMonster::CreateMonster();
            ZIdType monsterId = monster->GetId();
            delete monster;

            UNREGISTER_SERVICE(prz::utl::IUniqueIdRegistry);

            ASSERT_EQ(UniqueIdRegistryMonsterTestMock::kSomeId, monsterId);
        }
    }
}
