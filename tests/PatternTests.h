#pragma once
#include "../pch.h"
#include "../Pattern.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Pattern, aStripedPatternIsConstantInY) {
  const auto p = strip_pattern(WHITE, BLACK);
  EXPECT_EQ(pattern_at(p, point(0, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 1, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 2, 0)), WHITE);  
}

TEST(Pattern, aStripedPatternIsConstantInZ) {
  const auto p = strip_pattern(WHITE, BLACK);
  EXPECT_EQ(pattern_at(p, point(0, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 0, 1)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 0, 2)), WHITE);  
}

TEST(Pattern, aStripedPatternIsAlnternatesInX) {
  const auto p = strip_pattern(WHITE, BLACK);
  EXPECT_EQ(pattern_at(p, point(0, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0.9f, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(1, 0, 0)), BLACK);  

  EXPECT_EQ(pattern_at(p, point(-0.1f, 0, 0)), BLACK);
  EXPECT_EQ(pattern_at(p, point(-1.0f, 0, 0)), BLACK);
  EXPECT_EQ(pattern_at(p, point(-1.1f, 0, 0)), WHITE);  
}

TEST(Pattern, lightingWithPatternApplied) {
  /*give material a Pattern object*/
}

RESTORE_WARNINGS