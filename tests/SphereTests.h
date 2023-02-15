#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Sphere.h"

TEST(Sphere, hasTransform) {
    auto s = sphere(point(0, 0, 0), 1.0f);
    EXPECT_EQ(s.transform, Matrix4Identity);
}

TEST(Sphere, normalAtAPointOnXAxis) {
    auto s = sphere(point(0, 0, 0), 1.0f);
    const auto result = normal_at(s, point(1, 0, 0));
    const auto expected = vector(1, 0, 0);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalAtAPointOnYAxis) {
    auto s = sphere(point(0, 0, 0), 1.0f);
    const auto result = normal_at(s, point(0, 1, 0));
    const auto expected = vector(0, 1, 0);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalAtAPointOnZAxis) {
    auto s = sphere(point(0, 0, 0), 1.0f);
    const auto result = normal_at(s, point(0, 0, 1));
    const auto expected = vector(0, 0, 1);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalAtANonAxialPoint) {
    auto s = sphere(point(0, 0, 0), 1.0f);
    constexpr auto v = math::sqrt(3.0f) / 3.0f;
    const auto result = normal_at(s, point(v, v, v));
    const auto expected = vector(v, v, v);
    EXPECT_EQ(result, expected);
}
TEST(Sphere, normalIsANormalizedVector) {
    auto s = sphere(point(0, 0, 0), 1.0f);
    constexpr auto v = math::sqrt(3) / 3.0f;
    const auto result = normal_at(s, point(v, v, v));
    const auto expected = normalize(result);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalOnTranslatedSphere) {
    auto s = sphere(point(0, 0, 0), 1.0f);
    s.transform = translation(0, 1, 0);    
    const auto result = normal_at(s, point(0.0f, 1.70711f, -0.70711f));
    const auto expected = vector(0.0f, 0.70711f, -0.70711f);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalOnTransformedSphere) {
    constexpr auto v = math::sqrt(2) / 2.0f;
    auto s = sphere(point(0, 0, 0), 1.0f);
    s.transform = scaling(1.0f, 0.5f, 1.0f) * rotation_z(math::PI/5.0f); 
    const auto result = normal_at(s, point(0.0f, v, v));
    const auto expected = vector(0.0f, 0.97014f, -0.24254f);
    EXPECT_EQ(result, expected);
}