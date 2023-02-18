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


