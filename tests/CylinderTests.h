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
    auto r = ray(point(1, 0, -5), vector(0, 0, 1));
    auto xs = local_intersect(c,  r);
    EXPECT_FLOAT_EQ(xs.first, 5.0f);
    EXPECT_FLOAT_EQ(xs.second, 5.0f);

    r = ray(point(0, 0, -5), vector(0, 0, 1)); 
    xs = local_intersect(c,  r);
    EXPECT_FLOAT_EQ(xs.first, 4.0f);
    EXPECT_FLOAT_EQ(xs.second, 6.0f);

    r = ray(point(0.5f, 0, -5), vector(0.1f, 1, 1)); 
    xs = local_intersect(c,  r);    
    EXPECT_FLOAT_EQ(xs.first, 6.80798f); //actual 4.8019881
    EXPECT_FLOAT_EQ(xs.second, 7.08872f);    //4.9999919
}

TEST(Cylinder, rayMissesACylinder) {
    const std::vector<Ray> rays{
        {point(1, 0, 0), vector(0, 1, 0)},
        {point(0, 0, 0), vector(0, 1, 0)},
        {point(0, 0, -5), vector(1, 1, 1)}        
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
        point(1, 0.5f, -0.8f),
        point(-1, -0.2f, 0.9f),
        point(-0.4f, 1, -0.1f),
        point(0.3, -1, -0.7f),
        point(-0.6f, 0.3f, 1),
        point(0.4f, 0.4f, -1),
        point(1, 1, 1),
        point(-1, -1, -1)
    };
    const std::vector<Vector> normals{
        vector(1, 0, 0),
        vector(-1, 0, 0),
        vector(0, 1, 0),
        vector(0, -1, 0),
        vector(0, 0, 1),
        vector(0, 0, -1),
        vector(1, 0, 0),
        vector(-1, 0, 0)
    };
 /*   for (size_t i = 0; i < points.size(); i++) {
        const auto p = points[i];
        const auto normal = local_normal_at(c, p);
        EXPECT_EQ(normal, normals[i]);
        
    }*/
}

RESTORE_WARNINGS