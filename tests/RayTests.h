#pragma once
#include "../pch.h"
#include "../Ray.h"



TEST(Ray, CanBeConstructed) {
    const auto origin = point(1, 2, 3);
    const auto direction = vector(4,5,6);
    const auto r = ray(origin, direction);
    
    EXPECT_EQ(r.origin, origin);
    EXPECT_EQ(r.direction, direction);
}

TEST(Ray, canComputePointFromDistance) {
    const auto r = ray(point(2,3,4), vector(1,0,0));    
    EXPECT_EQ(position(r, 0), point(2,3,4));
    EXPECT_EQ(position(r, 1), point(3,3,4));
    EXPECT_EQ(position(r, -1), point(1,3,4));
    EXPECT_EQ(position(r, 2.5f), point(4.5,3,4));

    EXPECT_EQ(r.position(0), point(2,3,4));
    EXPECT_EQ(r.position(1), point(3,3,4));
    EXPECT_EQ(r.position(-1), point(1,3,4));
    EXPECT_EQ(r.position(2.5f), point(4.5,3,4));
}

TEST(Ray, intersectSphereAtTwoPoints) {
    const auto r = ray(point(0,0,-5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.count, 2);
    EXPECT_FLOAT_EQ(xs[0], 4.0f);
    EXPECT_FLOAT_EQ(xs[1], 6.0f);
}

TEST(Ray, intersectSphereAtATangent) {
    const auto r = ray(point(0,1,-5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.count, 2);
    EXPECT_FLOAT_EQ(xs[0], 5.0f);
    EXPECT_FLOAT_EQ(xs[1], 5.0f);
}

TEST(Ray, intersectMissingASphere) {
    const auto r = ray(point(0,2,-5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.count, 0);
}

TEST(Ray, intersectWhenOriginInsideSphere) {
    const auto r = ray(point(0,0,0), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.count, 2);
    EXPECT_FLOAT_EQ(xs[0], -1.0f); //ray extends backwards! critical for reflections. 
    EXPECT_FLOAT_EQ(xs[1], 1.0f);
}

TEST(Ray, intersectWhenSphereBehindRay) {
    const auto r = ray(point(0,0,5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.count, 2);
    EXPECT_FLOAT_EQ(xs[0], -6.0f); //ray extends backwards! critical for reflections. 
    EXPECT_FLOAT_EQ(xs[1], -4.0f);
}