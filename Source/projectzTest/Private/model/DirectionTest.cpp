#include "projectzTestPrivatePCH.h"

#include "gtest/gtest.h"

#include "model/Direction.h"

namespace prz {
    namespace testing {
        class DirectionTest : public ::testing::Test {
        protected:
            static const mdl::ZPositionDiff kForward;
            static const mdl::ZPositionDiff kLeft;
            static const mdl::ZPositionDiff kRight;
            static const mdl::ZPositionDiff kBackward;
            static const mdl::ZPositionDiff kHalfLeft;
            static const mdl::ZPositionDiff kHalfRight;

            mdl::ZDirection dir;
        };

        const mdl::ZPositionDiff DirectionTest::kForward(1, 0);
        const mdl::ZPositionDiff DirectionTest::kLeft(0, -1);
        const mdl::ZPositionDiff DirectionTest::kRight(0, 1);
        const mdl::ZPositionDiff DirectionTest::kBackward(-1, 0);
        const mdl::ZPositionDiff DirectionTest::kHalfLeft(1, -1);
        const mdl::ZPositionDiff DirectionTest::kHalfRight(1, 1);

        TEST_F(DirectionTest, DefaultDirectionPredictsForwardMove) {
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kForward.GetdX(), diff.GetdX());
            ASSERT_EQ(kForward.GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsLeftMove) {
            dir.RotateLeft();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kLeft.GetdX(), diff.GetdX());
            ASSERT_EQ(kLeft.GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsRightMove) {
            dir.RotateRight();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kRight.GetdX(), diff.GetdX());
            ASSERT_EQ(kRight.GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsBackwardMove) {
            dir.RotateBack();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kBackward.GetdX(), diff.GetdX());
            ASSERT_EQ(kBackward.GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsHalfLeftMove) {
            dir.RotateHalfLeft();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kHalfLeft.GetdX(), diff.GetdX());
            ASSERT_EQ(kHalfLeft.GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_PredictsHalfRightMove) {
            dir.RotateHalfRight();
            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kHalfRight.GetdX(), diff.GetdX());
            ASSERT_EQ(kHalfRight.GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_FourthRotateLeftsRotateToForward) {
            for (int i = 0; i < 4; ++i) {
                dir.RotateLeft();
            }

            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kForward.GetdX(), diff.GetdX());
            ASSERT_EQ(kForward.GetdY(), diff.GetdY());
        }

        TEST_F(DirectionTest, RotateLeft_400RotateLeftsRotateToForward) {
            for (int i = 0; i < 400; ++i) {
                dir.RotateLeft();
            }

            mdl::ZPositionDiff diff = dir.PredictMove();

            ASSERT_EQ(kForward.GetdX(), diff.GetdX());
            ASSERT_EQ(kForward.GetdY(), diff.GetdY());
        }
    }
}
