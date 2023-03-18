#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Cylinder.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Cylinder, rayIntersectsACylinder) {    
    const auto c = cylinder();    
    auto r = ray(point(1, 0, -5), vector(0, 0, 1)); //hit cylinder at a tangent 
    auto xs = local_intersect(c,  r);
    EXPECT_FLOAT_EQ(xs.first, 5.0f);
    EXPECT_FLOAT_EQ(xs.second, 5.0f);

    r = ray(point(0, 0, -5), vector(0, 0, 1)); //hit cylinder perpendicular, through the middle.
    xs = local_intersect(c,  r);
    EXPECT_FLOAT_EQ(xs.first, 4.0f);
    EXPECT_FLOAT_EQ(xs.second, 6.0f);

    //hit the cylinder at an angle
    r = ray(point(0.5f, 0, -5), vector(0.1f, 1, 1)); //the vector is from the book, but must be normalized for this test to match even a little bit.
    xs = local_intersect(c,  r);    
    EXPECT_FLOAT_EQ(xs.first, 6.8080058f); //book oracle: 6.80798f
    EXPECT_FLOAT_EQ(xs.second, 7.0886984f); //book oracle: 7.08872f
}

TEST(Cylinder, rayMissesACylinder) {
    const std::vector<Ray> rays{
        {point(1, 0, 0), vector(0, 1, 0)}, //on the surface, pointing up. Ergo: perpendicular to the cylinder
        {point(0, 0, 0), vector(0, 1, 0)}, //inside the the cylinder, pointing up
        {point(0, 0, -5), vector(1, 1, 1)} //far away from the cylinder, pointer wherever
    };    
    const auto c = cylinder();
    for (size_t i = 0; i < rays.size(); i++) {        
        const auto xs = local_intersect(c, rays[i]);
        EXPECT_FLOAT_EQ(xs.first, 0);
        EXPECT_FLOAT_EQ(xs.second, 0);
    }
}


TEST(Cylinder, normalOnTheSurfaceOfACylinder) {   
    const std::vector<Point> points{
        point(1, 0, 0),
        point(0, 5, -1),
        point(0, -2, 1),
        point(-1, 1, 0)        
    };
    const std::vector<Vector> normals{
        vector(1, 0, 0),
        vector(0, 0, -1),
        vector(0, 0, 1),
        vector(-1, 0, 0)        
    };
    const auto c = cylinder();
    for (size_t i = 0; i < points.size(); i++) {
        const auto p = points[i];
        const auto normal = local_normal_at(c, p);
        EXPECT_EQ(normal, normals[i]);        
    }
}

RESTORE_WARNINGS