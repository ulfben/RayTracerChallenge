#pragma once
#include "../pch.h"
#include "../Ray.h"
#include "../Intersection.h"
#include "../Shapes.h"

DISABLE_WARNINGS_FROM_GTEST

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
}

TEST(Ray, intersectSphereAtTwoPoints) {
    const auto r = ray(point(0,0,-5), vector(0,0,1));    
    const auto s = sphere();
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 4.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 6.0f);
}

TEST(Ray, intersectSphereAtATangent) {
    const auto r = ray(point(0,1,-5), vector(0,0,1));    
    const auto s = sphere();
    const auto xs = intersect(s, r);

    EXPECT_TRUE(xs);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 5.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 5.0f);
}

TEST(Ray, intersectMissingASphere) {
    const auto r = ray(point(0,2,-5), vector(0,0,1));    
    const auto s = sphere();
    const auto xs = intersect(s, r);
    EXPECT_FALSE(xs);
    EXPECT_EQ(xs.size(), 0);
}

TEST(Ray, intersectWhenOriginInsideSphere) {
    const auto r = ray(ORIGO, vector(0,0,1));    
    const auto s = sphere();
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, -1.0f); //ray extends backwards! critical for reflections. 
    EXPECT_FLOAT_EQ(xs[1].t, 1.0f);
}

TEST(Ray, intersectWhenSphereBehindRay) {
    const auto r = ray(point(0,0,5), vector(0,0,1));    
    const auto s = sphere();
    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, -6.0f); //ray extends backwards! critical for reflections. 
    EXPECT_FLOAT_EQ(xs[1].t, -4.0f);
}

TEST(Intersection, encapsulatesTandObject) {    
    const Shapes s{ std::in_place_type<Sphere> };
    const auto i = intersection(3.5f, s);
    EXPECT_TRUE(i);
    EXPECT_EQ(i, i);    
    EXPECT_FLOAT_EQ(i.t, 3.5f);    
    EXPECT_EQ(i.object(), s);
}

TEST(Intersections, aggregatesIntersection) {    
    const auto s = sphere();
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
    const Sphere s = sphere();
    const auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 4.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 6.0f);
    EXPECT_TRUE(xs[0]);    
    EXPECT_EQ(*xs[0].objPtr, s);
    EXPECT_EQ(*xs[1].objPtr, s);
}

TEST(closest, returnsClosestIntersection) {    
    const auto s = sphere();
    const auto i1 = intersection(1, s);
    const auto i2 = intersection(2, s);
    const auto xs = intersections(i1, i2);
    EXPECT_TRUE(xs);
    const auto i = closest(xs);    
    EXPECT_EQ(i, i1);
}

TEST(closest, returnsClosestPositiveIntersection) {    
    const auto s = sphere();
    const auto i1 = intersection(-1, s);
    const auto i2 = intersection(1, s);
    const auto xs = intersections(i1, i2);
    const auto i = closest(xs);    
    EXPECT_EQ(i, i2);
}

TEST(closest, returnsEmptyIntersectionIfAllAreNegative) {    
    const auto s = sphere();
    const auto i1 = intersection(-2, s);
    const auto i2 = intersection(-1, s);
    const auto xs = intersections(i1, i2);
    const auto i = closest(xs);    
    EXPECT_FLOAT_EQ(i.t, 0.0f);
    EXPECT_FALSE(i);
}

TEST(closest, canHandleMultipleIntersections) {    
    const auto s = sphere();
    const auto i1 = intersection(5, s);
    const auto i2 = intersection(7, s);
    const auto i3 = intersection(-3, s);
    const auto i4 = intersection(2, s);
    const auto i5 = intersection(-4, s);
    const auto xs = intersections({ i1, i2, i3, i4, i5 });
    const auto i = closest(xs);    
    EXPECT_EQ(i, i4);    
}

TEST(Ray, canBeTranslated) {
    const auto r = ray(point(1,2,3), vector(0,1,0));    
    const auto m = translation(3, 4, 5);
    const auto r2 = m * r;    
    const auto r3 = r * m;   
    EXPECT_EQ(r2.origin, point(4, 6, 8));
    EXPECT_EQ(r2.direction, vector(0,1,0));
    EXPECT_EQ(r2, r3);
}
TEST(Ray, canBeScaled) {
    const auto r = ray(point(1,2,3), vector(0,1,0));    
    const auto m = scaling(2, 3, 4);
    const auto r2 = transform(r, m);    
    EXPECT_EQ(r2.origin, point(2, 6, 12));
    EXPECT_EQ(r2.direction, vector(0,3,0));
}

TEST(intersect, aScaledSphere) {    
    const auto r = ray(point(0,0,-5), vector(0,0,1));
    const auto s = sphere(scaling(2, 2, 2));
    const auto xs = intersect(s, r);    
    EXPECT_EQ(xs.count(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 3.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 7.0f);    
}

//TEST(intersect, aScaledShape) {    
//    const auto r = ray(point(0,0,-5), vector(0,0,1));
//    auto s = sphere()
//    s.transform = scaling(2, 2, 2);    
//    const auto xs = intersect(s, r);    
//    EXPECT_EQ(s.saved_ray.origin, point(0, 0, -2.5f));
//    EXPECT_EQ(s.saved_ray.direction, vector(0, 0, 0.5f));    
//
//    auto p = plane();
//    p.transform = scaling(2, 2, 2);    
//    const auto xs1 = intersect(p, r);    
//    EXPECT_EQ(p.saved_ray.origin, point(0, 0, -2.5f));
//    EXPECT_EQ(p.saved_ray.direction, vector(0, 0, 0.5f));    
//}
//
//TEST(intersect, aTranslatedShape) {    
//    const auto r = ray(point(0,0,-5), vector(0,0,1));
//    auto s = sphere()
//    s.transform = translation(5,0,0);    
//    const auto xs = intersect(s, r);    
//    EXPECT_EQ(s.saved_ray.origin, point(-5.0f, 0, -5.0f));
//    EXPECT_EQ(s.saved_ray.direction, vector(0, 0, 1.0f));    
//
//    auto p = plane();
//    p.transform = scaling(2, 2, 2);    
//    const auto xs1 = intersect(p, r);    
//    EXPECT_EQ(p.saved_ray.origin, point(-5.0f, 0, -5.0f));
//    EXPECT_EQ(p.saved_ray.direction, vector(0, 0, 1.0f));    
//}

TEST(Intersections, precomputeIntersectionState) {    
    const auto r = ray(point(0,0,-5), vector(0,0,1));
    const Shapes s = sphere();
    const auto i1 = intersection(4, s);
    const auto hit = prepare_computations(i1, r);
    EXPECT_FLOAT_EQ(i1.t, hit.t);    
    EXPECT_EQ(hit.object(), s);
    EXPECT_EQ(hit.point, point(0, 0, -1));
    EXPECT_EQ(hit.eye_v, vector(0, 0, -1));
    EXPECT_EQ(hit.normal, vector(0, 0, -1));
}

TEST(Intersections, stateIncludesInside) {    
    const auto r = ray(point(0,0,-5), vector(0,0,1));
    const Shapes s = sphere();
    const auto i1 = intersection(4, s);
    const auto hit = prepare_computations(i1, r);
    EXPECT_FALSE(hit.inside);        
}

TEST(Intersections, stateIncludesInside2) {    
    const auto r = ray(point(0,0, 0), vector(0,0,1));
    const Shapes s = sphere();
    const auto i1 = intersection(1, s);
    const auto hit = prepare_computations(i1, r);
    EXPECT_TRUE(hit.inside);
    EXPECT_EQ(hit.point, point(0, 0, 1));
    EXPECT_EQ(hit.eye_v, vector(0, 0, -1));
    EXPECT_EQ(hit.normal, vector(0, 0, -1));
}

TEST(Intersections, stateIncludesOverPoint) {
    const auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    Shapes sh = sphere(translation(0,0,1));   
    const auto i1 = intersection(5, sh);
    const auto hit = prepare_computations(i1, r);
    EXPECT_TRUE(hit);
    EXPECT_LT(hit.over_point.z, -std::numeric_limits<Real>::epsilon()/2); //half of -EPSILON, to make sure over_point has been adjusted in the correct direction    
    EXPECT_GT(hit.point.z, hit.over_point.z); 
}

TEST(Intersections, stateIncludesUnderPoint) {
    const auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    Shapes sh = sphere(glass(), translation(0,0,1));   
    const auto i1 = intersection(5, sh);
    const auto xs = intersections({ i1 });
    const auto hit = prepare_computations(i1, r, xs);
    EXPECT_TRUE(hit);
    EXPECT_GT(hit.under_point.z, std::numeric_limits<Real>::epsilon()/2.0f);
    EXPECT_LT(hit.point.z, hit.under_point.z); 
}

RESTORE_WARNINGS