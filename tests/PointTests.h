#pragma once
#include "../pch.h"
#include "../Tuple.h"

DISABLE_WARNINGS_FROM_GTEST

//point - point = vector
//point + point is illegal
//point + vector = vctor
//point * scalar is illegal

TEST(Point, canBeDefaultConstructed) {
  const Point a;
  EXPECT_FLOAT_EQ(a.x, .0f);
  EXPECT_FLOAT_EQ(a.y, .0f);
  EXPECT_FLOAT_EQ(a.z, .0f);
}

TEST(Point, minusPointIsVector) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = point(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));  
}

TEST(Point, pointPlusVectorIsPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 + t2;
  EXPECT_TRUE(res == point(8, 8, 8));  
}
TEST(Point, minusVectorIsAPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5, 6, 7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == point(-2, -4, -6));  
}TEST(Point, canBeNegated) {
  const auto t1 = point(3, 2, 1);  ;
  EXPECT_EQ(-t1, point(-3, -2, -1));  
}
RESTORE_WARNINGS