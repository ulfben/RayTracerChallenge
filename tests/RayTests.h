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