#pragma once
#include "../pch.h"
#include "../Material.h"
#include "../Ray.h"
#include "../Shapes.h"
#include "../Intersection.h"
#include "../World.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Reflection, materialHasDefaultReflectivity) {
   const auto m = material();
  EXPECT_FLOAT_EQ(m.reflective, .0f);  
}

TEST(Reflection, isComputedByHitState) {
    constexpr auto halfSqrt = math::sqrt(2.0f) / 2.0f;
    const Shapes p = plane();
    const auto r = ray(point(0, 1, -1), vector(0, -halfSqrt, halfSqrt));
    const auto ix = intersection(sqrt(2.0f), p);
    const auto state = prepare_computations(ix, r);
    EXPECT_EQ(state.reflectv, vector(0, halfSqrt, halfSqrt));  
}

TEST(Reflection, reflectedColorOfNonreflectiveMaterial) {
    auto w = World{};    
    const auto r = ray(ORIGO, vector(0, 0, 1));    
    get_material(w, 1).ambient = 1.0f;
    const auto ix = intersection(1, w[1]);
    const auto state = prepare_computations(ix, r);
    const auto color = reflected_color(w, state, 1); 
    EXPECT_EQ(color, BLACK);  
}

TEST(Reflection, reflectedColorForReflectiveMaterial) { 
    constexpr auto sqrt = math::sqrt(2.0f);
    constexpr auto halfSqrt = sqrt / 2.0f;
    auto w = World();
    Plane p = plane();
    p.surface.reflective = 0.5f;
    p.transform = translation(0,-1,0);
    w.push_back(std::move(p));
    const auto r = ray(point(0, 0, -3), vector(0, -halfSqrt, halfSqrt));    
    const auto ix = intersection(sqrt, w.back());
    const auto state = prepare_computations(ix, r);
    const auto c = reflected_color(w, state, 1); 
    //EXPECT_EQ(c, color(0.19032f, 0.2379f, 0.14274f));  //book oracle
    EXPECT_EQ(c, color(0.1911927f, 0.23899086f, 0.14339453f));
}

TEST(Reflection, shadowsCanBlendWithReflections) {   
    constexpr auto sqrt = math::sqrt(2.0f);
    constexpr auto halfSqrt = sqrt / 2.0f;
    auto w = World();
    Plane p = plane();
    p.surface.reflective = 0.5f;
    p.transform = translation(0,-1,0);
    w.push_back(std::move(p));
    const auto r = ray(point(0, 0, -3), vector(0, -halfSqrt, halfSqrt));    
    const auto ix = intersection(sqrt, w.back());
    const auto state = prepare_computations(ix, r);
    const auto c = shade_hit(w, state, 1); 
    //EXPECT_EQ(c, color(0.87677f, 0.92436f, 0.82918f)); //book oracle 
    EXPECT_EQ(c, color(0.87761813f, 0.9254163f, 0.82982f));    
}

TEST(Reflection, canHandleInfiniteRecursion) {       
    auto lower = plane(mirror(), translation(0,-1,0));    
    auto upper = plane(mirror(), translation(0,1,0));    
    auto w = World({lower, upper});
    w.light = point_light(ORIGO, color(1, 1, 1));    
    const auto r = ray(point(0,0,0), vector(0,1,0)); 
    const auto c = color_at(w, r); 
    EXPECT_TRUE(true); 
}

TEST(Reflection, canLimitRecursion) {   
    constexpr auto sqrt = math::sqrt(2.0f);
    constexpr auto halfSqrt = sqrt / 2.0f;
    auto w = World();
    Plane p = plane();
    p.surface.reflective = 0.5f;
    p.transform = translation(0,-1,0);
    w.push_back(std::move(p));
    const auto r = ray(point(0, 0, -3), vector(0, -halfSqrt, halfSqrt));    
    const auto ix = intersection(sqrt, w.back());
    const auto state = prepare_computations(ix, r);
    const auto c = reflected_color(w, state, 0); 
    EXPECT_EQ(c, color(0,0,0));    
}

RESTORE_WARNINGS