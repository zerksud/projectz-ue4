#include "projectzTestPrivatePCH.h"

#include "gtest/gtest.h"

#include "model/Direction.h"

namespace prz {
    namespace testing {
        class DirectionTest : public ::testing::Test {
        protected:
            static const mdl::ZPositionDiff kPositionDiffs[];

            mdl::ZDirection dir;
        };

        const mdl::ZPositionDiff DirectionTest::kPositionDiffs[] = {
            mdl::ZPositionDiff(1, 0),
            mdl::ZPositionDiff(0, -1),
            mdl::ZPositionDiff(0, 1),
            mdl::ZPositionDiff(-1, 0),
            mdl::ZPositionDiff(1, -1),
            mdl::ZPositionDiff(1, 1)
        };

        namespace Direction {
            enum Type {
                Forward = 0,
                Left,
                Right,
                Backward,
                HalfLeft,
                HalfRight
            };
        }

        TEST_F(DirectionTest, DefaultDirectionPredictsForwardMove) {
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::Forward].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::Forward].GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsLeftMove) {
            dir.RotateLeft();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::Left].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::Left].GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsRightMove) {
            dir.RotateRight();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::Right].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::Right].GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsBackwardMove) {
            dir.RotateBack();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::Backward].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::Backward].GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsHalfLeftMove) {
            dir.RotateHalfLeft();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::HalfLeft].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::HalfLeft].GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsHalfRightMove) {
            dir.RotateHalfRight();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::HalfRight].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::HalfRight].GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_FourthRotateLeftsRotateToForward) {
            for (int i = 0; i < 4; ++i) {
                dir.RotateLeft();
            }

            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::Forward].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::Forward].GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_400RotateLeftsRotateToForward) {
            for (int i = 0; i < 400; ++i) {
                dir.RotateLeft();
            }

            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kPositionDiffs[Direction::Forward].GetdX(), diff.GetdX());
            ASSERT_EQ(kPositionDiffs[Direction::Forward].GetdY(), diff.GetdY());
        }
    }
}
