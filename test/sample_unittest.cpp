#include "gtest/gtest.h"

int add_func(int a, int b) {
  return a + b;
}

TEST(SampleTest, Test) {
  EXPECT_EQ(10, add_func(4, 6));
}
