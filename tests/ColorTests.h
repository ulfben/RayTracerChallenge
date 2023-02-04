#pragma once
#include "../pch.h"
#include "../Tuple.h"

TEST(color, canBeDefaultConstructed) {
  const Tuple a;
  EXPECT_FLOAT_EQ(a.r, .0f);
  EXPECT_FLOAT_EQ(a.g, .0f);
  EXPECT_FLOAT_EQ(a.b, .0f);
  EXPECT_FLOAT_EQ(a.a, .0f);
}

TEST(color, canBeConstructed) {
  const auto a = color(-0.5f, 0.4f, 1.7f);
  EXPECT_FLOAT_EQ(a.r, -0.5f);
  EXPECT_FLOAT_EQ(a.g, 0.4f);
  EXPECT_FLOAT_EQ(a.b, 1.7f);
  EXPECT_FLOAT_EQ(a.a, 1.0f);
}

TEST(color, canBeMultiplied) {
  const auto a = color(1.0f, 0.2f, 0.4f);
  const auto b = color(0.9f, 1.0f, 0.1f);
  const auto result = a * b;
  const auto resultHadamard = hadamard_product(a, b);
  EXPECT_EQ(result, resultHadamard);
  EXPECT_FLOAT_EQ(result.r, 0.9f);
  EXPECT_FLOAT_EQ(result.g, 0.2f);
  EXPECT_FLOAT_EQ(result.b, 0.04f);
  EXPECT_FLOAT_EQ(result.a, 1.0f);
}