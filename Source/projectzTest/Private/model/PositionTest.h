#include "gtest/gtest.h"

#include "model/Position.h"

#define ASSERT_POSITION_EQ(expectedPosition, actualPosition) \
    EXPECT_EQ(expectedPosition##.GetX(), actualPosition##.GetX()); \
    ASSERT_EQ(expectedPosition##.GetY(), actualPosition##.GetY());
