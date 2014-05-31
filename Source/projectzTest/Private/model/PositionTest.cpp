#include "projectzTestPrivatePCH.h"

#include "model/PositionTest.h"
#include "utils/StringHelpers.h"

namespace prz {
    namespace testing {
        class PositionTest : public ::testing::Test {
        protected:
            static const int kDefaultValue = 0;
            static const int kSomeValue = 42;
            static const int kAnotherValue = 1337;
            static const int kValuesDiff = kSomeValue - kAnotherValue;
        };

        TEST_F(PositionTest, DefaultConstructorInitializesXYWithZeroes) {
            mdl::ZPosition pos;

            ASSERT_EQ(kDefaultValue, pos.GetX());
            ASSERT_EQ(kDefaultValue, pos.GetY());
        }

        TEST_F(PositionTest, ConstructorWithXYParametersUsesThemToInitializeXY) {
            mdl::ZPosition pos(kSomeValue, kAnotherValue);

            ASSERT_EQ(kSomeValue, pos.GetX());
            ASSERT_EQ(kAnotherValue, pos.GetY());
        }

        TEST_F(PositionTest, CopyConstructorCopiesPosition) {
            mdl::ZPosition pos(kSomeValue, kAnotherValue);
            mdl::ZPosition anotherPos(pos);

            ASSERT_EQ(kSomeValue, anotherPos.GetX());
            ASSERT_EQ(kAnotherValue, anotherPos.GetY());
        }

        TEST_F(PositionTest, PositionDiffDefaultConstructorInitializesXYWithZeroes) {
            mdl::ZPositionDiff diff;

            ASSERT_EQ(kDefaultValue, diff.GetdX());
            ASSERT_EQ(kDefaultValue, diff.GetdY());
        }

        TEST_F(PositionTest, PositionDiffConstructorWithXYParametersUsesThemToInitializeXY) {
            mdl::ZPositionDiff diff(kSomeValue, kAnotherValue);

            ASSERT_EQ(kSomeValue, diff.GetdX());
            ASSERT_EQ(kAnotherValue, diff.GetdY());
        }

        TEST_F(PositionTest, OperatorMinusPositionCalculatesDifference) {
            mdl::ZPosition pos(kSomeValue, kAnotherValue);
            mdl::ZPosition anotherPos(kAnotherValue, kSomeValue);

            mdl::ZPositionDiff posDiff = pos - anotherPos;

            ASSERT_EQ(kValuesDiff, posDiff.GetdX());
            ASSERT_EQ(-kValuesDiff, posDiff.GetdY());
        }

        TEST_F(PositionTest, AssignmentOperatorCopiesPosition) {
            mdl::ZPosition pos;
            mdl::ZPosition anotherPos(kSomeValue, kAnotherValue);

            pos = anotherPos;

            ASSERT_EQ(kSomeValue, pos.GetX());
            ASSERT_EQ(kAnotherValue, pos.GetY());
        }

        TEST_F(PositionTest, OperatorPlusDiffCalculatesSum) {
            mdl::ZPosition pos(kSomeValue, kAnotherValue);
            mdl::ZPositionDiff diff(kSomeValue, kAnotherValue);

            pos = pos + diff;

            ASSERT_EQ(kSomeValue * 2, pos.GetX());
            ASSERT_EQ(kAnotherValue * 2, pos.GetY());
        }

        TEST_F(PositionTest, OperatorEqualsReturnsTrueForEqualPosition) {
            mdl::ZPosition pos(kSomeValue, kAnotherValue);
            mdl::ZPosition anotherPos(kSomeValue, kAnotherValue);

            bool equal = pos == anotherPos;

            ASSERT_TRUE(equal);
        }

        TEST_F(PositionTest, OperatorEqualsReturnsFalseForNonEqualPosition) {
            mdl::ZPosition pos(kSomeValue, kSomeValue);
            mdl::ZPosition anotherPos(kAnotherValue, kAnotherValue);

            bool equal = pos == anotherPos;

            ASSERT_FALSE(equal);
        }

        TEST_F(PositionTest, Position_ToStringReturnsStringInSomeFormat) {
            mdl::ZPosition pos(kSomeValue, kAnotherValue);
            std::string expectedString = utl::ZString::Format("[%d;%d]", kSomeValue, kAnotherValue);

            ASSERT_EQ(expectedString, pos.ToString());
        }

        TEST_F(PositionTest, PositionDiff_ToStringReturnsStringInSomeFormat) {
            mdl::ZPositionDiff diff(kSomeValue, kAnotherValue);
            std::string expectedString = utl::ZString::Format("[%d;%d]", kSomeValue, kAnotherValue);

            ASSERT_EQ(expectedString, diff.ToString());
        }
    }
}
