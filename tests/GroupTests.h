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
    g.push_back(&s);
    EXPECT_TRUE(g.front() == s);
    EXPECT_TRUE(get_parent(g.front()) == &g);
}

TEST(Group, IntersectingRayWithEmptyGroup) {           
    Group g;
    auto r = ray(point(0, 0, 0), vector(0, 0, 1));
    auto xs = local_intersect(g, r);
    EXPECT_TRUE(xs.empty());
}

TEST(Group, IntersectingRayWithNonEmptyGroup) {           
    Group g;
    Shapes s1 = sphere();
    Shapes s2 = sphere();
    set_transform(s2, translation(0, 0, -3));
    Shapes s3 = sphere();
    set_transform(s3, translation(5, 0, 0));
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto xs = local_intersect(g, r);
    EXPECT_EQ(xs.size(), 4);
    xs[0] == s2;
    xs[1] == s2;
    xs[2] == s1;
    xs[3] == s1;
}

RESTORE_WARNINGS
