#pragma once
#include "../pch.h"
#include "../Math.h"
#include "../Matrix.h"
#include "../Camera.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Camera, canBeConstructed) {
    const auto c = Camera(160, 120, math::PI / 2.0f);
    EXPECT_EQ(c.width, 160);
    EXPECT_EQ(c.height, 120);    
    EXPECT_FLOAT_EQ(c.field_of_view, math::PI / 2);
    EXPECT_EQ(c.transform, Matrix4Identity);
}

TEST(Camera, calculatesPixelSizeForHorizontalCanvas) {
    const auto c = Camera(200, 125, math::PI / 2.0f);
    EXPECT_EQ(c.width, 200);
    EXPECT_EQ(c.height, 125);
    EXPECT_FLOAT_EQ(c.pixel_size, 0.01f);    
}

TEST(Camera, calculatesPixelSizeForVerticalCanvas) {
    const auto c = Camera(125, 200, math::PI / 2.0f);
    EXPECT_EQ(c.width, 125);
    EXPECT_EQ(c.height, 200);
    EXPECT_FLOAT_EQ(c.pixel_size, 0.01f);
}

TEST(Camera, constructRayThroughCenterOfCanvas) {
    const auto c = Camera(201, 101, math::PI / 2.0f);
    const auto r = ray_for_pixel(c, 100, 50);
    EXPECT_EQ(r.origin, point(0,0,0));
    EXPECT_EQ(r.direction, vector(0,0,-1));
}

TEST(Camera, constructRayThroughCornerOfCanvas) {
    const auto c = Camera(201, 101, math::PI / 2.0f);
    const auto r = ray_for_pixel(c, 0, 0);
    EXPECT_EQ(r.origin, ORIGO);
    EXPECT_EQ(r.direction, vector(0.66519f, 0.33259f, -0.66851f));
}

TEST(Camera, constructRayWithCameraTransformed) {
    const auto sqr = std::sqrtf(2.0f) / 2.0f;
    auto c = Camera(201, 101, math::PI / 2.0f);
    c.transform = rotation_y(math::PI / 4.0f) * translation(0,-2,5);
    const auto r = ray_for_pixel(c, 100, 50);
    EXPECT_EQ(r.origin, point(0, 2, -5));
    EXPECT_EQ(r.direction, vector(sqr, 0, -sqr));
}

TEST(Camera, renderWorldWithCamera) {
    const auto w = World();    
    auto c = Camera(11, 11, math::PI / 2.0f);
    const auto from = point(0, 0, -5);
    const auto to = ORIGO;
    const auto up = point(0, 1, 0);
    c.transform = view_transform(from, to, up);    
    const auto img = render(c, w);
    EXPECT_EQ(img.width(), c.height);
    EXPECT_EQ(img.height(), c.width);
    EXPECT_EQ(img.get(5,5), color(0.38066f, 0.47583f, 0.2855f));    
}

RESTORE_WARNINGS