#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Cube.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Cube, rayIntersectsACube) {
    const auto c = cube();
    EXPECT_EQ(c.transform(), Matrix4Identity);
    EXPECT_EQ(c.inv_transform(), inverse(c.transform()));

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
    for (size_t i = 0; i < rays.size(); i++) {
        const auto r = rays[i];
        const auto xs = local_intersect(c, r);        
        EXPECT_FLOAT_EQ(xs.first, expected[i].first);
        EXPECT_FLOAT_EQ(xs.second, expected[i].second);
    }
}

RESTORE_WARNINGS