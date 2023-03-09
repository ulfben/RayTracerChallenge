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

TEST(Transparency, theRefractedColorWithAnOpaqueSurface) {
    const auto w = World();
    const auto& shape = w[0];
    const auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    const auto xs = intersections(intersection(4.0f, shape), intersection(6.0f,shape));
    const auto state = prepare_computations(xs[0], r, xs);
    const auto c = refracted_color(w, state, 5);
    EXPECT_EQ(c, BLACK); 
}


TEST(Transparency, theRefractedColorAtMaximumRecursiveDepth) {
    auto w = World();
    auto& shape = w[0];
    surface(shape).transparency = 1.0f;
    surface(shape).refractive_index = 1.5f;
    const auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    const auto xs = intersections(intersection(4.0f, shape), intersection(6.0f,shape));
    const auto state = prepare_computations(xs[0], r, xs);
    const auto c = refracted_color(w, state, 0);
    EXPECT_EQ(c, BLACK); 
}

TEST(Transparency, theRefractedColorUnderTotalInternalReflection) {
    constexpr auto halfSqrt = math::sqrt(2.0f) / 2.0f;
    auto w = World();
    auto& shape = w[0];
    surface(shape).transparency = 1.0f;
    surface(shape).refractive_index = 1.5f;
    const auto r = ray(point(0, 0,halfSqrt), vector(0, 1, 0));
    const auto xs = intersections(intersection(-halfSqrt, shape), intersection(halfSqrt,shape));
    const auto state = prepare_computations(xs[1], r, xs);
    const auto c = refracted_color(w, state, 5);
    EXPECT_EQ(c, BLACK); 
}

TEST(Transparency, theRefractedColorWithRefractedRay) {
   
    EXPECT_TRUE(false); 
}

TEST(Transparency, shadeHitWithTransparentMaterial) {
    constexpr auto sqrt = math::sqrt(2.0f);
    constexpr auto halfSqrt = sqrt / 2.0f;
    auto w = World();
    auto floor = plane(translation(0,-1,0));
    floor.surface.transparency = 0.5f;
    floor.surface.refractive_index = 1.5f;

    auto ball = sphere(translation(0, -3.5f, -0.5f));
    ball.surface.color = color(1, 0, 0);
    ball.surface.ambient = 0.5f;

    w.push_back(floor); 
    w.push_back(ball);        
    
    const auto r = ray(point(0, 0,-3.0f), vector(0, -halfSqrt, halfSqrt));
    const auto xs = intersections({ intersection(sqrt, floor) });
    const auto state = prepare_computations(xs[0], r, xs);
    const auto c = shade_hit(w, state, 5);
    //color(0.93642f, 0.68462f, 0.68462f) //book oracle     
    EXPECT_EQ(c, color(0.93642545f, 0.68642545f, 0.68642545f)); 
}

RESTORE_WARNINGS