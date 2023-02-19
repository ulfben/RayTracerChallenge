#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Lights.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Color, canBeDefaultConstructed) {
  const Color a;
  EXPECT_FLOAT_EQ(a.r, .0f);
  EXPECT_FLOAT_EQ(a.g, .0f);
  EXPECT_FLOAT_EQ(a.b, .0f);  
}

TEST(Color, canBeConstructed) {
  const auto a = color(-0.5f, 0.4f, 1.7f);
  EXPECT_FLOAT_EQ(a.r, -0.5f);
  EXPECT_FLOAT_EQ(a.g, 0.4f);
  EXPECT_FLOAT_EQ(a.b, 1.7f);  
}

TEST(Color, canBeMultiplied) {
  const auto a = color(1.0f, 0.2f, 0.4f);
  const auto b = color(0.9f, 1.0f, 0.1f);
  const auto result = a * b;
  const auto resultHadamard = hadamard_product(a, b);
  EXPECT_EQ(result, resultHadamard);
  EXPECT_FLOAT_EQ(result.r, 0.9f);
  EXPECT_FLOAT_EQ(result.g, 0.2f);
  EXPECT_FLOAT_EQ(result.b, 0.04f);  
}

RESTORE_WARNINGS