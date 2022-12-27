#include "pch.h"

using Real = float;
struct Tuple {
  Real x{};
  Real y{};
  Real z{};
  Real w{};  
  constexpr Tuple operator+(Tuple rhs) const noexcept {
    return Tuple{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
  }
  constexpr Tuple operator-(Tuple rhs) const noexcept {
    return Tuple{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
  }
  constexpr Tuple operator-() const noexcept {
    return Tuple{-x, -y, -z, w};
  }
  constexpr bool operator==(const Tuple& rhs) const noexcept = default;
};
constexpr bool is_vector(Tuple t) noexcept { return t.w == 0; }
constexpr bool is_point(Tuple t) noexcept { return t.w == 1; }

constexpr Tuple vector(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 0.0f};
}
constexpr Tuple point(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 1.0f};
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
  const Tuple truth{1, 1, 6, 1};
  EXPECT_TRUE(res == truth);
}

TEST(Tuples, pointPlusVectorIsPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5,6,7);
  const auto res = t1 + t2;
  EXPECT_TRUE(res == point(8, 8, 8));
  EXPECT_TRUE(is_point(res));
}

TEST(Tuples, vectorPlusVectorisVector) {
  const auto t1 = vector(3, 2, 1);
  const auto t2 = vector(5,6,7);
  const auto res = t1 + t2;
  EXPECT_TRUE(res == vector(8, 8, 8));
  EXPECT_TRUE(is_vector(res));
}

TEST(Tuples, pointMinusPointIsVector) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = point(5,6,7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));
  EXPECT_TRUE(is_vector(res));
}

TEST(Tuples, vectorMinusPointIsAPoint) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5,6,7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == point(-2, -4, -6));
  EXPECT_TRUE(is_point(res));
}

TEST(Tuples, vectorMinusVectorIsaVector) {
  const auto t1 = vector(3, 2, 1);
  const auto t2 = vector(5,6,7);
  const auto res = t1 - t2;
  EXPECT_TRUE(res == vector(-2, -4, -6));
  EXPECT_TRUE(is_vector(res));
}

TEST(Tuples, canBeNegated) {
  const auto t1 = point(3, 2, 1);
  const auto t2 = vector(5,6,7);
  EXPECT_EQ(-t1, point(-3, -2, -1));
  EXPECT_EQ(-t2, vector(-5, -6, -7));
}