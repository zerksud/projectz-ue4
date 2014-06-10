#include <algorithm>

#include "gtest/gtest.h"

#define EXPECT_CONTAINS(container, value) \
    EXPECT_TRUE(std::find((container).begin(), (container).end(), value) != (container).end());

#define ASSERT_CONTAINS(container, value) \
    ASSERT_TRUE(std::find((container).begin(), (container).end(), value) != (container).end());
