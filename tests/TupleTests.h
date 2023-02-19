#pragma once
#include "../pch.h"
#include "../Tuple.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Tuple, canBeDefaultConstructed) {
  const Tuple a;
  EXPECT_FLOAT_EQ(a.x, .0f);
  EXPECT_FLOAT_EQ(a.y, .0f);
  EXPECT_FLOAT_EQ(a.z, .0f);
  EXPECT_FLOAT_EQ(a.w, .0f);
}

TEST(Tuple, W1isPoint) {
  const Tuple t{4.3f, -4.2f, 3.1f, 1.0f};
  EXPECT_EQ(t.x, 4.3f);
  EXPECT_EQ(t.y, -4.2f);
  EXPECT_EQ(t.z, 3.1f);
  EXPECT_TRUE(is_point(t));
  EXPECT_FALSE(is_vector(t));
}

TEST(Tuple, W0isVector) {
  const Tuple t{4.3f, -4.2f, 3.1f, 0.0f};
  EXPECT_EQ(t.x, 4.3f);
  EXPECT_EQ(t.y, -4.2f);
  EXPECT_EQ(t.z, 3.1f);
  EXPECT_FALSE(is_point(t));
  EXPECT_TRUE(is_vector(t));
}
TEST(Tuple, vectorFactoryMakesVectors) {
  const Tuple t = vector(4.3f, -4.2f, 3.1f);
  EXPECT_FALSE(is_point(t));
  EXPECT_TRUE(is_vector(t));
}
TEST(Tuple, pointFactoryMakesPoints) {
  const Tuple t = point(4.3f, -4.2f, 3.1f);
  EXPECT_TRUE(is_point(t));
  EXPECT_FALSE(is_vector(t));
}

TEST(Tuples, canBeAdded) {
  const Tuple t1{3, -2, 5, 1};
  const Tuple t2{-2, 3, 1, 0};
  const Tuple res = t1 + t2;
  const Tuple expected{1, 1, 6, 1};
  EXPECT_TRUE(res == expected);
}

TEST(Tuples, pointPlusVectorIsPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 + t2;
  EXPECT_TRUE(res == point(8, 8, 8));
  EXPECT_TRUE(is_point(res));
}

TEST(Tuples, vectorPlusVectorisVector) {
  const auto t1 = vector(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 + t2;
  EXPECT_TRUE(res == vector(8, 8, 8));
  EXPECT_TRUE(is_vector(res));
}

TEST(Tuples, pointMinusPointIsVector) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = point(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));
  EXPECT_TRUE(is_vector(res));
}

TEST(Tuples, vectorMinusPointIsAPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == point(-2, -4, -6));
  EXPECT_TRUE(is_point(res));
}

TEST(Tuples, vectorMinusVectorIsaVector) {
  const auto t1 = vector(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));
  EXPECT_TRUE(is_vector(res));
}

TEST(Tuples, canBeNegated) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  EXPECT_EQ(-t1, point(-3, -2, -1));
  EXPECT_EQ(-t2, vector(-5, -6, -7));
}

TEST(Tuples, canBeMultiplied) {
  const auto t1 = Tuple{3, 2, 1, 1};
  const auto t2 = vector(5, 6, 7);
  const auto expected = Tuple{9, 6, 3, 3};
  EXPECT_EQ(t1 * 3, expected);
  EXPECT_EQ(t2 * 3, vector(15, 18, 21));
}

TEST(Tuples, canBeMultipliedByFraction) {
  const auto t1 = Tuple{1, -2, 3, -4};
  const auto expected = Tuple{0.5, -1, 1.5, -2};
  EXPECT_EQ(t1 * 0.5f, expected);
}

TEST(Tuples, canBeDividedByScalar) {
  const auto t1 = Tuple{1, -2, 3, -4};
  const auto expected = Tuple{0.5, -1, 1.5, -2};
  EXPECT_EQ(t1 / 2, expected);
}

RESTORE_WARNINGS