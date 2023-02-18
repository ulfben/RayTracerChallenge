#pragma once
#include "../pch.h"
#include "../World.h"
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

