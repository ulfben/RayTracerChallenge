#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Sphere.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Sphere, hasTransform) {
    const auto s = sphere();
    EXPECT_EQ(s.transform(), Matrix4Identity);
    EXPECT_EQ(s.inv_transform(), inverse(s.transform()));
}

TEST(Sphere, normalAtAPointOnXAxis) {
    constexpr auto s = sphere();
    const auto result = normal_at(s, point(1, 0, 0));
    const auto expected = vector(1, 0, 0);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalAtAPointOnYAxis) {
    constexpr auto s = sphere();
    const auto result = normal_at(s, point(0, 1, 0));
    const auto expected = vector(0, 1, 0);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalAtAPointOnZAxis) {
    constexpr auto s = sphere();
    const auto result = normal_at(s, point(0, 0, 1));
    const auto expected = vector(0, 0, 1);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalAtANonAxialPoint) {
    constexpr auto s = sphere();
    constexpr auto v = math::sqrt(3.0f) / 3.0f;
    const auto result = normal_at(s, point(v, v, v));
    const auto expected = vector(v, v, v);
    EXPECT_EQ(result, expected);
}
TEST(Sphere, normalIsANormalizedVector) {
    constexpr auto s = sphere();
    constexpr auto v = math::sqrt(3.0f) / 3.0f;
    const auto result = normal_at(s, point(v, v, v));
    const auto expected = normalize(result);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalOnTranslatedSphere) {
    const auto s = sphere(translation(0, 1, 0));
    const auto result = normal_at(s, point(0.0f, 1.70711f, -0.70711f));
    const auto expected = vector(0.0f, 0.70711f, -0.70711f);
    EXPECT_EQ(result, expected);
}

TEST(Sphere, normalOnTransformedSphere) {
    constexpr auto v = math::sqrt(2) / 2.0f;
    auto s = sphere(scaling(1.0f, 0.5f, 1.0f) * rotation_z(math::PI/5.0f));
    const auto result = normal_at(s, point(0.0f, v, -v));
    const auto book_expected = vector(0.0f, 0.97014f, -0.24254f); //book oracle
    EXPECT_EQ(result, book_expected);
    const auto expected = vector(0.0f, 0.97014254f, -0.24253564f); //actual
    EXPECT_EQ(result, expected);
}

TEST(Sphere, hasADefaultMaterial) {    
    constexpr auto s = sphere();    
    EXPECT_EQ(s.surface, material());
}

TEST(Sphere, canBeAssignedMaterial) {    
    auto s = sphere();    
    auto surface = material();
    surface.ambient = 1.0f;
    s.surface = surface;
    EXPECT_EQ(s.surface, surface);
}

RESTORE_WARNINGS