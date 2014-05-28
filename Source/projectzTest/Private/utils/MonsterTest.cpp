#include "projectzTestPrivatePCH.h"

#include "model/Monster.h"

#include "model/PositionTest.h"

namespace prz {
    namespace testing {

        class MonsterTest : public ::testing::Test {
        protected:
            static const mdl::ZPositionDiff kForwardPositionDiff;
            static const mdl::ZPositionDiff kBackwardPositionDiff;

            mdl::ZMonster monster;
        };

        const mdl::ZPositionDiff MonsterTest::kForwardPositionDiff = mdl::ZPositionDiff(1, 0);
        const mdl::ZPositionDiff MonsterTest::kBackwardPositionDiff = mdl::ZPositionDiff(-1, 0);
        

        TEST_F(MonsterTest, Conctructor_DefaultMonsterDiretionIsForward) {
            mdl::ZPositionDiff expectedMove = monster.GetDirection().PredictMove();
            ASSERT_POSITION_DIFF_EQ(kForwardPositionDiff, expectedMove);
        }

        TEST_F(MonsterTest, HoldsDirectionBetweenCalls) {
            mdl::ZDirection& direction = monster.GetDirection();
            direction.RotateBack();

            mdl::ZPositionDiff expectedMove = monster.GetDirection().PredictMove();
            ASSERT_POSITION_DIFF_EQ(kBackwardPositionDiff, expectedMove);
        }
    }
}
