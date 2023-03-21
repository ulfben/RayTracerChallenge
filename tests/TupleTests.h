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

TEST(Point, pointPlusVectorIsPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 + t2;
  EXPECT_TRUE(res == point(8, 8, 8));  
}

TEST(Vector, vectorPlusVectorisVector) {
  const auto t1 = vector(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 + t2;
  EXPECT_TRUE(res == vector(8, 8, 8));  
}

TEST(Point, pointMinusPointIsVector) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = point(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));  
}

TEST(Vector, vectorMinusPointIsAPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == point(-2, -4, -6));  
}

TEST(Vector, vectorMinusVectorIsaVector) {
  const auto t1 = vector(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));  
}

TEST(Point, canBeNegated) {
  const auto t1 = point(3, 2, 1);  ;
  EXPECT_EQ(-t1, point(-3, -2, -1));  
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

RESTORE_WARNINGS