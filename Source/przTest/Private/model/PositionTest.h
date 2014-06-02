#include "gtest/gtest.h"

#include "model/Position.h"

#define ASSERT_POSITION_EQ(expectedPosition, actualPosition) \
    EXPECT_EQ(expectedPosition##.GetX(), actualPosition##.GetX()); \
    ASSERT_EQ(expectedPosition##.GetY(), actualPosition##.GetY());

#define ASSERT_POSITION_DIFF_EQ(expectedPositionDiff, actualPositionDiff) \
    EXPECT_EQ(expectedPositionDiff##.GetdX(), actualPositionDiff##.GetdX()); \
    ASSERT_EQ(expectedPositionDiff##.GetdY(), actualPositionDiff##.GetdY());
