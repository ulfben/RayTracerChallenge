#pragma once
#include "../pch.h"
#include "../World.h"
#include "../Ray.h"

TEST(World, hasDefaultWorld) {
  const auto w = World();
  auto s0 = sphere();
  auto s1 = sphere(World::DEFAULT_WORLD_MATERIAL); 
  auto s2 = sphere();
  s2.transform = scaling(0.5f, 0.5f, 0.5f);    
  EXPECT_FALSE(w.contains(s0));
  EXPECT_TRUE(w.contains(s1));
  EXPECT_TRUE(w.contains(s2));
}

TEST(World, canIntersectWithRay) {
  const auto w = World();
  const auto r = ray(point(0,0,-5), vector(0,0,1)); 
  auto xs = intersect(w, r);
  //two nestled spheres at origo, two hits each
  EXPECT_EQ(xs.count(), 4);
  EXPECT_EQ(xs[0].t, 4.0f);
  EXPECT_EQ(xs[1].t, 4.5f);
  EXPECT_EQ(xs[2].t, 5.5f);
  EXPECT_EQ(xs[3].t, 6.0f);
}

TEST(World, shadeAnIntersection) {
    const auto w = World();
    const auto r = ray(point(0, 0, -5), vector(0, 0, 1));    
    const auto i = intersection(4, w[0]);
    const auto comps = prepare_computations(i, r);
    const auto c = shade_hit(w, comps);    
    EXPECT_EQ(c, color(0.38066f, 0.47583f, 0.2855f));    
}

TEST(World, shadeAnIntersectionFromInside) {
    auto w = World();
    w.light = point_light(point(0, 0.25f, 0), color(1, 1, 1));
    const auto r = ray(point(0, 0, 0), vector(0, 0, 1));    
    const auto i = intersection(0.5f, w[1]);
    const auto comps = prepare_computations(i, r);
    const auto col = shade_hit(w, comps);
    EXPECT_EQ(col, color(0.90498f, 0.90498f, 0.90498f));
}