#pragma once
#include "../pch.h"
#include "../Tuple.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Point, canBeDefaultConstructed) {
  const Point a;
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
  //const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto expected = point(9, 6, 3);
  //EXPECT_EQ(t1 * 3, expected);
  EXPECT_EQ(t2 * 3, vector(15, 18, 21));
}

//TEST(Tuples, canBeMultipliedByFraction) {
//  const auto t1 = Tuple{1, -2, 3, -4};
//  const auto expected = Tuple{0.5, -1, 1.5, -2};
//  EXPECT_EQ(t1 * 0.5f, expected);
//}

//TEST(Tuples, canBeDividedByScalar) {
//  const auto t1 = Tuple{1, -2, 3, -4};
//  const auto expected = Tuple{0.5, -1, 1.5, -2};
//  EXPECT_EQ(t1 / 2, expected);
//}

RESTORE_WARNINGS