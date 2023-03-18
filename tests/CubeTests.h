#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Cube.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Cube, hasTransformAndInverseTransform) {
    const auto c = cube();
    EXPECT_EQ(c.transform(), Matrix4Identity);
    EXPECT_EQ(c.inv_transform(), inverse(c.transform()));
}

TEST(Cube, rayIntersectsACube) {
    const std::vector<Ray> rays{
        {point(5, 0.5f, 0), vector(-1, 0, 0)}, /* +x */
        {point(-5, 0.5f, 0), vector(1, 0, 0)}, /* -x */
        {point(0.5f, 5, 0), vector(0, -1, 0)}, /* +y */
        {point(0.5f, -5, 0), vector(0, 1, 0)}, /* -y */
        {point(0.5f, 0, 5), vector(0,0,-1)},   /* +z */
        {point(0.5f, 0, -5), vector(0, 0, 1)}, /* -z */
        {point(0, 0.5f, 0), vector(0, 0, 1)}   /* inside */
    };
    const std::vector<std::pair<Real, Real>> expected{
        {4.0f, 6.0f},
        {4.0f, 6.0f},
        {4.0f, 6.0f},
        {4.0f, 6.0f},
        {4.0f, 6.0f},
        {4.0f, 6.0f},
        {-1.0f, 1.0f}
    };
    const auto c = cube();    
    for (size_t i = 0; i < rays.size(); i++) {                     
        const auto xs = local_intersect(c, rays[i]); 
        EXPECT_FLOAT_EQ(xs.first, expected[i].first);
        EXPECT_FLOAT_EQ(xs.second, expected[i].second);        
    }
}

TEST(Cube, rayMissesACube) {
    const auto c = cube();
    const std::vector<Ray> rays{
        {point(-2, 0, 0), vector(0.2673f, 0.5345f, 0.8018f)},
        {point(0, -2, 0), vector(0.8018f, 0.2673f, 0.5345f)},
        {point(0, 0, -2), vector(0.5345F, 0.8018f, 0.2673f)},
        {point(2, 0, 2), vector(0, 0, -1)},
        {point(0, 2, 2), vector(0,-1,0)},  
        {point(2, 2, 0), vector(-1, 0, 0)}
    };    
    for (size_t i = 0; i < rays.size(); i++) {                
        const auto xs = local_intersect(c, rays[i]);
        EXPECT_FLOAT_EQ(xs.first, 0);
        EXPECT_FLOAT_EQ(xs.second, 0);
    }
}


TEST(Cube, normalOnTheSurfaceOfACube) {
    const auto c = cube();
    const std::vector<Point> points{
        point(1, 0.5f, -0.8f),
        point(-1, -0.2f, 0.9f),
        point(-0.4f, 1, -0.1f),
        point(0.3f, -1, -0.7f),
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
    for (size_t i = 0; i < points.size(); i++) {
        const auto p = points[i];
        const auto normal = local_normal_at(c, p);
        EXPECT_EQ(normal, normals[i]);
        
    }
}

RESTORE_WARNINGS