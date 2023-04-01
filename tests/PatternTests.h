#pragma once
#include "../pch.h"
#include "../Pattern.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Pattern, testPatternWithAnObjectTransform) {
    auto shape = sphere();
    shape.set_transform(scaling(2, 2, 2));
    const auto p = test_pattern();
    const auto c = pattern_at(p, shape, point(2, 3, 4));
    EXPECT_EQ(c, color(1, 1.5f, 2));    
}

//page 134
TEST(Pattern, testPatternWithAPatternTransform) {
    const auto shape = sphere();    
    auto p = test_pattern();
    p.set_transform(scaling(2, 2, 2));
    const auto c = pattern_at(p, shape, point(2, 3, 4));
    EXPECT_EQ(c, color(1, 1.5f, 2));    
}

TEST(Pattern, testPatternWithBothObjectAndAPatternTransform) {
    auto shape = sphere();
    shape.set_transform(scaling(2, 2, 2));  
    auto p = test_pattern();
    p.set_transform(translation(0.5f, 1, 1.5f));
    const auto c = pattern_at(p, shape, point(2.5f, 3, 3.5f));
    EXPECT_EQ(c, color(0.75f, 0.5f, 0.25f));    
}

TEST(Pattern, aStripedPatternIsConstantInY) {
  const auto p = stripe_pattern(WHITE, BLACK);
  EXPECT_EQ(pattern_at(p, point(0, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 1, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 2, 0)), WHITE);  
}

TEST(Pattern, aStripedPatternIsConstantInZ) {
  const auto p = stripe_pattern(WHITE, BLACK);
  EXPECT_EQ(pattern_at(p, point(0, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 0, 1)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0, 0, 2)), WHITE);  
}

TEST(Pattern, aStripedPatternIsAlnternatesInX) {
  const auto p = stripe_pattern(WHITE, BLACK);
  EXPECT_EQ(pattern_at(p, point(0, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(0.9f, 0, 0)), WHITE);
  EXPECT_EQ(pattern_at(p, point(1, 0, 0)), BLACK);  

  EXPECT_EQ(pattern_at(p, point(-0.1f, 0, 0)), BLACK);
  EXPECT_EQ(pattern_at(p, point(-1.0f, 0, 0)), BLACK);
  EXPECT_EQ(pattern_at(p, point(-1.1f, 0, 0)), WHITE);  
}

TEST(Pattern, lightingWithPatternApplied) {
    auto m = material();
    m.pattern = stripe_pattern(WHITE, BLACK);
    m.ambient = 1.0f;
    m.diffuse = 0.0f;
    m.specular = 0.0f;
    const auto eyev = vector(0, 0, -1);
    const auto normalv = vector(0, 0, -1);
    const auto light = point_light(point(0, 0, -10), WHITE);
    EXPECT_EQ(lighting(m, light, point(0.9f, 0, 0), eyev, normalv, false), WHITE);
    EXPECT_EQ(lighting(m, light, point(1.1f, 0, 0), eyev, normalv, false), BLACK);    
}

TEST(Pattern, stripesWithAnObjectTransform) {
    const auto obj = sphere(scaling(2, 2, 2));    
    const auto pattern = stripe_pattern(WHITE, BLACK);    
    EXPECT_EQ(pattern_at(pattern, obj, point(1.5f, 0, 0)), WHITE);    
}

TEST(Pattern, stripesWithAnPatternTransform) {
    const auto obj = sphere();    
    const auto pattern = stripe_pattern(WHITE, BLACK, scaling(2, 2, 2));
    EXPECT_EQ(pattern_at(pattern, obj, point(1.5f, 0, 0)), WHITE);    
}

TEST(Pattern, stripesWithBothAnObjectAndPatternTransform) {
    const auto obj = sphere(scaling(2, 2, 2));    
    const auto pattern = stripe_pattern(WHITE, BLACK, translation(0.5f, 0, 0));
    EXPECT_EQ(pattern_at(pattern, obj, point(2.5f, 0, 0)), WHITE);    
}

TEST(Pattern, gradientInterpolatesLinearly) {    ;    
    const auto pattern = gradient_pattern(WHITE, BLACK, translation(0.5f, 0, 0));
    EXPECT_EQ(pattern_at(pattern, point(0.0f, 0, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(0.25f, 0, 0)), color(0.75f, 0.75f, 0.75f));    
    EXPECT_EQ(pattern_at(pattern, point(0.5f, 0, 0)), color(0.5f, 0.5f, 0.5f));
    EXPECT_EQ(pattern_at(pattern, point(0.75, 0, 0)), color(0.25f, 0.25f, 0.25f));    
    EXPECT_EQ(pattern_at(pattern, point(0.99f, 0, 0)), color(0.00999999f, 0.00999999f, 0.00999999f));
    EXPECT_EQ(pattern_at(pattern, point(1.0f, 0, 0)), WHITE);
}

TEST(Pattern, ringPatternExtendInBothXandZ) {    ;    
    const auto pattern = ring_pattern(WHITE, BLACK);    
    EXPECT_EQ(pattern_at(pattern, point(0.0f, 0, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(1.0f, 0, 0)), BLACK);    
    EXPECT_EQ(pattern_at(pattern, point(0, 0, 1.0f)), BLACK);
    //0.708f, just slightly more than sqrt(2)/2
    EXPECT_EQ(pattern_at(pattern, point(0.708f, 0, 0.708f)), BLACK);        
}

TEST(Pattern, checkersShouldRepeatInX) {    ;    
    const auto pattern = checkers_pattern(WHITE, BLACK);    
    EXPECT_EQ(pattern_at(pattern, point(0.0f, 0, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(0.99f, 0, 0)), WHITE);    
    EXPECT_EQ(pattern_at(pattern, point(1.01f, 0, 0)), BLACK);    
}
TEST(Pattern, checkersShouldRepeatInY) {    ;    
    const auto pattern = checkers_pattern(WHITE, BLACK);    
    EXPECT_EQ(pattern_at(pattern, point(0.0f, 0, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(0, 0.99f, 0)), WHITE);    
    EXPECT_EQ(pattern_at(pattern, point(0, 1.01f, 0)), BLACK);    
}
TEST(Pattern, checkersShouldRepeatInZ) {    ;    
    const auto pattern = checkers_pattern(WHITE, BLACK);    
    EXPECT_EQ(pattern_at(pattern, point(0.0f, 0, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(0, 0, 0.99f)), WHITE);    
    EXPECT_EQ(pattern_at(pattern, point(0, 0, 1.01f)), BLACK);    
}

TEST(Pattern, checkersPatternIn2D) {    ;    
    const auto pattern = uv_checkers_pattern(2, 2, BLACK, WHITE);    
    EXPECT_EQ(pattern_at(pattern, point(0, 0, 0)), BLACK);
    EXPECT_EQ(pattern_at(pattern, point(0.5f, 0, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(0, 0.5f, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(0.5f, 0.5f, 0)), BLACK);
    EXPECT_EQ(pattern_at(pattern, point(1, 1, 0)), BLACK);
}

TEST(Pattern, TestSphericalMapping) {
    constexpr auto tolerance = math::BRAZZY_EPSILON;

    const auto [u1, v1] = spherical_map(point(0, 0, -1));
    EXPECT_NEAR(u1, 0.0f, tolerance);
    EXPECT_NEAR(v1, 0.5f, tolerance);

    const auto [u2, v2] = spherical_map(point(1, 0, 0));
    EXPECT_NEAR(u2, 0.25f, tolerance);
    EXPECT_NEAR(v2, 0.5f, tolerance);

    const auto [u3, v3] = spherical_map(point(0, 0, 1));
    EXPECT_NEAR(u3, 0.5f, tolerance);
    EXPECT_NEAR(v3, 0.5f, tolerance);

    const auto [u4, v4] = spherical_map(point(-1, 0, 0));
    EXPECT_NEAR(u4, 0.75f, tolerance);
    EXPECT_NEAR(v4, 0.5f, tolerance);

    const auto [u5, v5] = spherical_map(point(0, 1, 0));
    EXPECT_NEAR(u5, 0.5f, tolerance);
    EXPECT_NEAR(v5, 1.0f, tolerance);

    const auto [u6, v6] = spherical_map(point(0, -1, 0));
    EXPECT_NEAR(u6, 0.5f, tolerance);
    EXPECT_NEAR(v6, 0.0f, tolerance);

    const auto [u7, v7] = spherical_map(point(math::sqrt(2.0f) / 2.0f, math::sqrt(2.0f) / 2.0f, 0));
    EXPECT_NEAR(u7, 0.25f, tolerance);
    EXPECT_NEAR(v7, 0.75f, tolerance);
}

RESTORE_WARNINGS