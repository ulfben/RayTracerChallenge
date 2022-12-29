#include "pch.h"
#include "Tuple.h"
#include "Canvas.h"
#include "StringHelpers.h"

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

TEST(Canvas, hasWidthHeightAndColor) {
  const auto canvas = Canvas(10, 20);
  EXPECT_EQ(canvas.width(), (uint32_t) 10);
  EXPECT_EQ(canvas.height(), (uint32_t) 20);
  for (const auto &pixel : canvas) {
    EXPECT_EQ(pixel, color(.0f, .0f, .0f, 1.0f));
  }
}

TEST(Canvas, canSetAndGetPixel) {
  auto canvas = Canvas(10, 20);
  const auto red = color(1, 0, 0);
  canvas.set(2, 3, red);
  EXPECT_EQ(canvas.get(2,3), red);
}

TEST(Canvas, canOutputPPMHeader) {
  const auto canvas = Canvas(5, 3);
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  EXPECT_EQ(lines[0], "P3"sv);
  EXPECT_EQ(lines[1], "5 3"sv);
  EXPECT_EQ(lines[2], "255"sv);
}

TEST(Canvas, canOutputPPMPixelData) {
  auto canvas = Canvas(5, 3);
  canvas.set(0, 0, color(1.5f, 0, 0));
  canvas.set(2, 1, color(0, 0.5f, 0));
  canvas.set(4, 2, color(-0.5, 0, 1.0f));
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  EXPECT_EQ(lines[3], "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0"sv);
  EXPECT_EQ(lines[4], "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0"sv);
  EXPECT_EQ(lines[5], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255"sv);
}