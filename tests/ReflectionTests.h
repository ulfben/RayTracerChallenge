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
    const auto halfSqrt = math::sqrt(2.0f) / 2.0f;
    const Shapes p = plane();
    const auto r = ray(point(0, 1, -1), vector(0, -halfSqrt, halfSqrt));
    const auto ix = intersection(sqrt(2.0f), p);
    const auto state = prepare_computations(ix, r);
    EXPECT_EQ(state.reflectv, vector(0, halfSqrt, halfSqrt));  
}

TEST(Reflection, reflectedColorOfNonreflectiveMaterial) {
    auto w = World{};
    const Shapes p = plane();
    const auto r = ray(point(0, 0, 0), vector(0, 0, 1));
    get_material(w, 1).ambient = 1.0f;
    const auto ix = intersection(1, w[1]);
    const auto state = prepare_computations(ix, r);
    const auto color = reflected_color(w, state); 
    EXPECT_EQ(color, BLACK);  
}

RESTORE_WARNINGS