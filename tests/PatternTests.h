#pragma once
#include "../pch.h"
#include "../Pattern.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Pattern, canCreateStripedPatted) {
  const auto p = strip_pattern(WHITE, BLACK);
  /*EXPECT_EQ(p.a, WHITE);
  EXPECT_FLOAT_EQ(p.b, BLACK);  */
}


RESTORE_WARNINGS