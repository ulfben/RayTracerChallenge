#pragma once
#include "../pch.h"
#include "../Pattern.h"

DISABLE_WARNINGS_FROM_GTEST

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
    auto pattern = stripe_pattern(WHITE, BLACK);    
    EXPECT_EQ(pattern_at(pattern, obj, point(1.5f, 0, 0)), WHITE);    
}

TEST(Pattern, stripesWithAnPatternTransform) {
    const auto obj = sphere();    
    auto pattern = stripe_pattern(WHITE, BLACK);
    pattern.transform = scaling(2, 2, 2);
    EXPECT_EQ(pattern_at(pattern, obj, point(1.5f, 0, 0)), WHITE);    
}

TEST(Pattern, stripesWithBothAnObjectAndPatternTransform) {
    const auto obj = sphere(scaling(2, 2, 2));    
    auto pattern = stripe_pattern(WHITE, BLACK);
    pattern.transform = translation(0.5f, 0, 0);
    EXPECT_EQ(pattern_at(pattern, obj, point(2.5f, 0, 0)), WHITE);    
}

TEST(Pattern, gradientInterpolatesLinearly) {    ;    
    auto pattern = gradient_pattern(WHITE, BLACK);
    pattern.transform = translation(0.5f, 0, 0);
    EXPECT_EQ(pattern_at(pattern, point(0.0f, 0, 0)), WHITE);
    EXPECT_EQ(pattern_at(pattern, point(0.25f, 0, 0)), color(0.75f, 0.75f, 0.75f));    
    EXPECT_EQ(pattern_at(pattern, point(0.5f, 0, 0)), color(0.5f, 0.5f, 0.5f));
    EXPECT_EQ(pattern_at(pattern, point(0.75, 0, 0)), color(0.25f, 0.25f, 0.25f));    
    EXPECT_EQ(pattern_at(pattern, point(0.99f, 0, 0)), color(0.00999999f, 0.00999999f, 0.00999999f));
    EXPECT_EQ(pattern_at(pattern, point(1.0f, 0, 0)), WHITE);
}

RESTORE_WARNINGS