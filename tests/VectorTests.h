#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Math.h"

DISABLE_WARNINGS_FROM_GTEST

//vector + vector = vector
//vector - vector = vector
//vector - point is illegal
//vector + point is illegal

TEST(Vector, canBeDefaultConstructed) {
  const Vector a;
  EXPECT_FLOAT_EQ(a.x, .0f);
  EXPECT_FLOAT_EQ(a.y, .0f);
  EXPECT_FLOAT_EQ(a.z, .0f);
}

TEST(Vector, canBeAdded) {
  const Vector t1{3, -2, 5};
  const Vector t2{-2, 3, 1};
  const Vector res = t1 + t2;
  const Vector expected{1, 1, 6};
  EXPECT_TRUE(res == expected);
}

TEST(Vector, canBeNegated) {  
  const auto t2 = vector(5, 6, 7);  
  EXPECT_EQ(-t2, vector(-5, -6, -7));
}

TEST(Vector, canBeMultipliedByScalar) {  
  const auto t2 = vector(5, 6, 7);
  const auto expected = point(9, 6, 3);  
  EXPECT_EQ(t2 * 3, vector(15, 18, 21));
}

TEST(Vector, minusVectorIsaVector) {
  const auto t1 = vector(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));  
}

TEST(Vector, hasMagnitude) {
  auto v = vector(1, 0, 0);
  EXPECT_FLOAT_EQ(magnitude(v), 1.0f);
  v = vector(0, 1, 0);
  EXPECT_FLOAT_EQ(magnitude(v), 1.0f);
  v = vector(0, 0, 1);
  EXPECT_FLOAT_EQ(magnitude(v), 1.0f);
  v = vector(1, 2, 3);
  EXPECT_FLOAT_EQ(magnitude(v), math::sqrt(14.0f));
  v = vector(-1, -2, -3);
  EXPECT_FLOAT_EQ(magnitude(v), math::sqrt(14.0f));  
}

TEST(Vector, canBeNormalized) {
  const auto v = vector(4, 0, 0);
  EXPECT_EQ(normalize(v), vector(1, 0, 0));
  const auto norm = normalize(vector(1, 2, 3));
  EXPECT_FLOAT_EQ(magnitude(norm), 1.0f);
  EXPECT_FLOAT_EQ(norm.x, 1.0f / math::sqrt(14.0f)); // roughly 0.26726
  EXPECT_FLOAT_EQ(norm.y, 2.0f / math::sqrt(14.0f)); // roughly 0.53452
  EXPECT_FLOAT_EQ(norm.z, 3.0f / math::sqrt(14.0f)); // roughly 0.80178
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
  const auto result = reflect(v, n); 
  EXPECT_EQ(result,vector(1, 1, 0));
}
TEST(Vector, canBeReflectedAtSlantedAngle) {
  const auto v = vector(0, -1, 0);
  const auto n = vector(math::sqrt(2.0f)/2.0f, math::sqrt(2.0f)/2.0f, 0);
  const auto result = reflect(v, n);
  EXPECT_EQ(result,vector(1, 0, 0));
}

RESTORE_WARNINGS