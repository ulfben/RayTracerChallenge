#pragma once
#include "../pch.h"
#include "../Material.h"
#include "../Ray.h"
#include "../Shapes.h"
#include "../Intersection.h"
#include "../World.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Transparency, findingN1andN2AtVariousIntersections) {
    constexpr Shapes a = sphere(glass(1.5), scaling(2, 2, 2));
    constexpr Shapes b = sphere(glass(2.0), translation(0,0, -0.25f));
    constexpr Shapes c = sphere(glass(2.5), translation(0,0, 0.25f));

    constexpr  auto r = ray(point(0,0,-4), vector(0,0,1));
    const  auto xs = intersections({
                        intersection(2.0f, a), intersection(2.75f, b),
                        intersection(3.25f, c), intersection(4.75f, b),
                        intersection(5.25f, c), intersection(6.0f, a)
    });
    const std::vector<std::pair<Real, Real>> expected{
        {1.0f, 1.5f},
        {1.5f, 2.0f},
        {2.0f, 2.5f},
        {2.5f, 2.5f},
        {2.5f, 1.5f},
        {1.5f, 1.0f}
    };
    for (Intersections::size_type i = 0; i < xs.size(); i++) {
        const auto state = prepare_computations(xs[i], r, xs);
        EXPECT_FLOAT_EQ(state.n1, expected[i].first);
        EXPECT_FLOAT_EQ(state.n2, expected[i].second);
    }
}

RESTORE_WARNINGS