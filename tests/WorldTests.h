#pragma once
#include "../pch.h"
#include "../World.h"
#include "../Ray.h"
#include "../Intersection.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(World, hasDefaultWorld) {
  const auto w = World();
  const Shapes s0{ sphere() };
  const Shapes s1 = sphere(World::DEFAULT_MATERIAL); 
  const auto s2 = sphere(scaling(0.5f, 0.5f, 0.5f));
  EXPECT_TRUE(w.size() == 2);
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
    //w.light = point_light(point(-10, -10, -10), WHITE);
    const auto r = ray(point(0, 0, -5), vector(0, 0, 1));    
    const auto i = intersection(4, w[0]);
    const auto comps = prepare_computations(i, r);
    const auto c = shade_hit(w, comps);    
    EXPECT_EQ(c, color(0.38066f, 0.47583f, 0.2855f));    
}

TEST(World, shadeAnIntersectionFromInside) {
    auto w = World();
    w.light = point_light(point(0, 0.25f, 0), WHITE);
    const auto r = ray(ORIGO, vector(0, 0, 1));    
    const auto i = intersection(0.5f, w[1]);
    const auto comps = prepare_computations(i, r);
    const auto col = shade_hit(w, comps);
    //EXPECT_EQ(col, color(0.90498f, 0.90498f, 0.90498f)); //prior to shadows and SHADOW_BIAS offset. 
    EXPECT_EQ(col, color(0.90335f, 0.90335f, 0.90335f));
}

TEST(World, colorWhenRayMisses) {
    const auto w = World();
    const auto r = ray(point(0, 0, -5), vector(0, 1, 0));
    const auto col = color_at(w, r);
    EXPECT_EQ(col, BLACK);
}

TEST(World, colorWhenRayHit) {
    auto w = World();
    const auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    const auto col = color_at(w, r); 
    EXPECT_EQ(col, color(0.38066f, 0.47583f, 0.2855f));
}

TEST(World, colorWhenHitBehindRay) {
    auto w = World();
    get_material(w, 0).ambient = 1.0f; //outer sphere
    get_material(w, 1).ambient = 1.0f; //inner sphere
    const auto r = ray(point(0, 0, 0.75f), vector(0, 0, -1));
    const auto col = color_at(w, r);
    EXPECT_EQ(col,  get_material(w, 1).color);
}

RESTORE_WARNINGS