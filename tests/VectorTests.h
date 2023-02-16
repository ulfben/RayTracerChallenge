#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Math.h"
TEST(Vector, hasMagnitude) {
  auto v = vector(1, 0, 0);
  EXPECT_FLOAT_EQ(magnitude(v), 1.0f);
  v = vector(0, 1, 0);
  EXPECT_FLOAT_EQ(magnitude(v), 1.0f);
  v = vector(0, 0, 1);
  EXPECT_FLOAT_EQ(magnitude(v), 1.0f);
  v = vector(1, 2, 3);
  EXPECT_FLOAT_EQ(magnitude(v), math::sqrt(14));
  v = vector(-1, -2, -3);
  EXPECT_FLOAT_EQ(magnitude(v), math::sqrt(14));
  EXPECT_TRUE(is_vector(v));
}

TEST(Vector, canBeNormalized) {
  const auto v = vector(4, 0, 0);
  EXPECT_EQ(normalize(v), vector(1, 0, 0));
  const auto norm = normalize(vector(1, 2, 3));
  EXPECT_FLOAT_EQ(magnitude(norm), 1.0f);
  EXPECT_FLOAT_EQ(norm.x, 1.0f / std::sqrt(14.0f)); // roughly 0.26726
  EXPECT_FLOAT_EQ(norm.y, 2.0f / std::sqrt(14.0f)); // roughly 0.53452
  EXPECT_FLOAT_EQ(norm.z, 3.0f / std::sqrt(14.0f)); // roughly 0.80178
}

TEST(Vector, hasDotProduct) {
  const auto a = vector(1, 2, 3);
  const auto b = vector(2, 3, 4);
  EXPECT_FLOAT_EQ(dot(a, b), 20.0f);
}

TEST(Vector, hasCrossProduct) {
  const auto a = vector(1, 2, 3);
  const auto b = vector(2, 3, 4);
  EXPECT_EQ(cross(a, b), vector(-1, 2, -1));
  EXPECT_EQ(cross(b, a), vector(1, -2, 1));
}
TEST(Vector, canBeReflectedAt45DegAngle) {
  const auto v = vector(1, -1, 0);
  const auto n = vector(0, 1, 0);
  const auto result = reflect(v, n); //reflecting a vector approaching at 45 degrees angle
  EXPECT_EQ(result,vector(1, 1, 0));
}
TEST(Vector, canBeReflectedAtSlantedAngle) {
  const auto v = vector(0, -1, 0);
  const auto n = vector(math::sqrt(2)/2, math::sqrt(2)/2, 0);
  const auto result = reflect(v, n); //reflecting a vector approaching at 45 degrees angle
  EXPECT_EQ(result,vector(1, 0, 0));
}