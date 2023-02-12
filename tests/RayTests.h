#pragma once
#include "../pch.h"
#include "../Ray.h"

TEST(Ray, canBeConstructed) {
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

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 4.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 6.0f);
}

TEST(Ray, intersectSphereAtATangent) {
    const auto r = ray(point(0,1,-5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_TRUE(xs);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 5.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 5.0f);
}

TEST(Ray, intersectMissingASphere) {
    const auto r = ray(point(0,2,-5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);
    EXPECT_FALSE(xs);
    EXPECT_EQ(xs.size(), 0);
}

TEST(Ray, intersectWhenOriginInsideSphere) {
    const auto r = ray(point(0,0,0), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, -1.0f); //ray extends backwards! critical for reflections. 
    EXPECT_FLOAT_EQ(xs[1].t, 1.0f);
}

TEST(Ray, intersectWhenSphereBehindRay) {
    const auto r = ray(point(0,0,5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, -6.0f); //ray extends backwards! critical for reflections. 
    EXPECT_FLOAT_EQ(xs[1].t, -4.0f);
}

TEST(Intersection, encapsulatesTandObject) {    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto i = intersection(3.5f, s);
    EXPECT_TRUE(i);
    EXPECT_EQ(i, i);    
    EXPECT_FLOAT_EQ(i.t, 3.5f);    
    EXPECT_EQ(i.obj, s);
}

TEST(Intersections, aggregatesIntersection) {    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto i1 = intersection(1, s);
    const auto i2 = intersection(2, s);    
    const auto xs = intersections(i1, i2);
    EXPECT_TRUE(xs);    
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 1);
    EXPECT_EQ(xs[1].t, 2);
}

TEST(intersect, setsTheObjectOnTheIntersections) {
    const auto r = ray(point(0,0,-5), vector(0,0,1));    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 4.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 6.0f);
    EXPECT_TRUE(xs[0]);
    EXPECT_EQ(xs[0].obj, s);
    EXPECT_EQ(xs[1].obj, s);
}

TEST(hit, allIntersectionsHavePositiveT) {    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto i1 = intersection(1, s);
    const auto i2 = intersection(2, s);
    const auto xs = intersections(i1, i2);
    EXPECT_TRUE(xs);
    const auto i = hit(xs);    
    EXPECT_EQ(i, i1);
}

TEST(hit, someIntersectionsHaveNegativeT) {    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto i1 = intersection(-1, s);
    const auto i2 = intersection(1, s);
    const auto xs = intersections(i1, i2);
    const auto i = hit(xs);    
    EXPECT_EQ(i, i2);
}

TEST(hit, allIntersectionsHaveNegativeT) {    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto i1 = intersection(-2, s);
    const auto i2 = intersection(-1, s);
    const auto xs = intersections(i1, i2);
    const auto i = hit(xs);    
    EXPECT_FLOAT_EQ(i.t, 0.0f);
    EXPECT_FALSE(i);
}

TEST(hit, isAlwaysLowestPositiveIntersection) {    
    const auto s = sphere(point(0,0,0), 1.0f);
    const auto i1 = intersection(5, s);
    const auto i2 = intersection(7, s);
    const auto i3 = intersection(-3, s);
    const auto i4 = intersection(2, s);
    const auto i5 = intersection(-4, s);
    const auto xs = intersections({ i1, i2, i3, i4, i5 });
    const auto i = hit(xs);    
    EXPECT_EQ(i, i4);    
}