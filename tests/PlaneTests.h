#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Ray.h"
#include "../Plane.h"
#include "../Shapes.h"
#include "../Intersection.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Plane, hasTransform) {
    constexpr auto p = plane();
    EXPECT_EQ(p.get_transform(), Matrix4Identity);
}

TEST(Plane, normalIsConstantEverywhere) {
    constexpr auto p = plane();
    const auto n1 = local_normal_at(p, ORIGO);
    const auto n2 = local_normal_at(p, point(10, 0, -10));
    const auto n3 = local_normal_at(p, point(-5, 0, 150));
    EXPECT_EQ(n1, vector(0, 1, 0));
    EXPECT_EQ(n2, vector(0, 1, 0));
    EXPECT_EQ(n3, vector(0, 1, 0));
}

TEST(Plane, intersectWithParallelRay) {
    constexpr auto p = plane();
    const auto r = ray(point(0, 10, 0), vector(0, 0, 1));
    const auto xs = local_intersect(p, r);
    EXPECT_EQ(xs, MISS);        
}

TEST(Plane, intersectWithCoplanarRay) {
    constexpr auto p = plane();
    const auto r = ray(ORIGO, vector(0, 0, 1));
    const auto xs = local_intersect(p, r);
    EXPECT_EQ(xs, MISS);        
}

TEST(Plane, intersectWithRayFromAbove) {
    constexpr auto p = plane();
    const auto r = ray(point(0, 1, 0), vector(0,-1, 0));
    const auto xs = local_intersect(p, r);    
    EXPECT_EQ(xs[0], 1.0f);    
         
    const auto xs2 = intersect(p, r);
    EXPECT_FALSE(xs2.empty()); 
    EXPECT_EQ(xs2.size(), 1);       
    EXPECT_EQ(xs2.object_at(0), p);
}

TEST(Plane, intersectWithRayFromBelow) {
    constexpr auto p = plane();
    const auto r = ray(point(0, -1, 0), vector(0,1, 0));
    const auto xs = local_intersect(p, r);    
    EXPECT_EQ(xs[0], 1.0f);
    //EXPECT_EQ(xs[1], T_MISS);
 
    const auto xs2 = intersect(p, r);
    EXPECT_FALSE(xs2.empty()); 
    EXPECT_EQ(xs2.size(), 1);       
    EXPECT_EQ(xs2.object_at(0), p);
}

RESTORE_WARNINGS