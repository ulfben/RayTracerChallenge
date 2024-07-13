#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Group.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST


TEST(Group, ANewGroupIsEmpty) {           
    Group g;
    EXPECT_TRUE(g.empty());
    EXPECT_EQ(g.size(), 0);
    EXPECT_EQ(g.get_transform(), Matrix4Identity);
    EXPECT_EQ(g.inv_transform(), inverse(g.get_transform()));
}

TEST(Group, ANewGroupHasNoParent) {           
    Group g;
    EXPECT_FALSE(g.has_parent());
}

TEST(Group, CanParentShape) {           
    Group g;
    Shapes s = Sphere();
  //  add_child(g, s);
  //  get_parent(s) == &g;
    g.front() == s;
}

RESTORE_WARNINGS
