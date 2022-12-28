#include "pch.h"

struct Tuple; 
using Real = float;
using Vector = Tuple;
using Point = Tuple;
using Color = Tuple;
constexpr Tuple hadamard_product(const Color &a, const Color &b) noexcept;

struct Tuple {
  union {
    Real data[4];
    struct {
      Real x, y, z, w;
    };
    struct {
      Real r, g, b, a;
    };
  };
  
  constexpr Tuple operator*(const Tuple& color) const noexcept {
    return hadamard_product(*this, color);
  }

  constexpr Tuple operator*(Real scalar) const noexcept {
    return Tuple{x * scalar, y * scalar, z * scalar, w * scalar};
  }
  constexpr Tuple operator/(Real scalar) const noexcept {
    return Tuple{x / scalar, y / scalar, z / scalar, w / scalar};
  }
  constexpr Tuple operator+(Tuple rhs) const noexcept {
    return Tuple{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
  }
  constexpr Tuple operator-(Tuple rhs) const noexcept {
    return Tuple{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
  }
  constexpr Tuple operator-() const noexcept { return Tuple{-x, -y, -z, w}; }
  constexpr bool operator==(const Tuple& rhs) const noexcept {
    return rhs.x == x && rhs.y == y && rhs.z == z && rhs.w == w;
  };
};


constexpr bool is_vector(Tuple t) noexcept { return t.w == 0; }
constexpr bool is_point(Tuple t) noexcept { return t.w == 1; }

constexpr Tuple vector(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 0.0f};
}
constexpr Tuple point(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 1.0f};
}
constexpr Tuple color(Real r, Real g, Real b, Real a = 1.0f) noexcept {
  return Tuple{r, g, b, a};
}

constexpr Real magnitudeSq(const Tuple &t) noexcept {
  return ((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Real magnitude(const Tuple &t) noexcept {
  return math::sqrt((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Tuple normalize(const Tuple &t) noexcept {
  const auto length = magnitude(t);
  assert(length != 0.0f);
  return t / length;
}
constexpr Real dot(const Vector &a, const Vector &b) noexcept {
  /*
 the smaller the dot product, the larger th angle between the vector
 if the two vectors are unit vectors, the dot product is the cosine of the angle
 between them
 */
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
constexpr Tuple cross(const Tuple &a, const Tuple &b) noexcept {
  return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

constexpr Tuple hadamard_product(const Color &a, const Color &b) noexcept {
  return Color{a.r * b.r, 
               a.g * b.g, 
               a.b * b.b,
               a.a * b.a};
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
  const auto truth = Tuple{9, 6, 3, 3};
  EXPECT_EQ(t1 * 3, truth);
  EXPECT_EQ(t2 * 3, vector(15, 18, 21));
}

TEST(Tuples, canBeMultipliedByFraction) {
  const auto t1 = Tuple{1, -2, 3, -4};
  const auto truth = Tuple{0.5, -1, 1.5, -2};
  EXPECT_EQ(t1 * 0.5f, truth);
}

TEST(Tuples, canBeDividedByScalar) {
  const auto t1 = Tuple{1, -2, 3, -4};
  const auto truth = Tuple{0.5, -1, 1.5, -2};
  EXPECT_EQ(t1 / 2, truth);
}

TEST(vector, hasMagnitude) {
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

TEST(vector, canBeNormalized) {
  const auto v = vector(4, 0, 0);
  EXPECT_EQ(normalize(v), vector(1, 0, 0));
  const auto norm = normalize(vector(1, 2, 3));
  EXPECT_FLOAT_EQ(magnitude(norm), 1.0f);
  EXPECT_FLOAT_EQ(norm.x, 1.0f / std::sqrt(14.0f)); // roughly 0.26726
  EXPECT_FLOAT_EQ(norm.y, 2.0f / std::sqrt(14.0f)); // roughly 0.53452
  EXPECT_FLOAT_EQ(norm.z, 3.0f / std::sqrt(14.0f)); // roughly 0.80178
}

TEST(vector, hasDotProduct) {
  const auto a = vector(1, 2, 3);
  const auto b = vector(2, 3, 4);
  EXPECT_FLOAT_EQ(dot(a, b), 20.0f);
}

TEST(vector, hasCrossProduct) {
  const auto a = vector(1, 2, 3);
  const auto b = vector(2, 3, 4);
  EXPECT_EQ(cross(a, b), vector(-1, 2, -1));
  EXPECT_EQ(cross(b, a), vector(1, -2, 1));
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
