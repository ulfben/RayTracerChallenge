#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Cube.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Cube, hasTransformAndInverseTransform) {
    const auto c = cube();
    EXPECT_EQ(c.get_transform(), Matrix4Identity);
    EXPECT_EQ(c.inv_transform(), inverse(c.get_transform()));
}

TEST(Cube, rayIntersectsACube) {
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
    const auto c = cube();
    for (size_t i = 0; i < rays.size(); i++) {
        const auto xs = local_intersect(c, rays[i]);
        EXPECT_FLOAT_EQ(xs[0], expected[i].first);
        EXPECT_FLOAT_EQ(xs[1], expected[i].second);
    }
}

TEST(Cube, rayMissesACube) {
    const auto c = cube();
    const std::vector<Ray> rays{
        {point(-2, 0, 0), vector(0.2673f, 0.5345f, 0.8018f)},
        {point(0, -2, 0), vector(0.8018f, 0.2673f, 0.5345f)},
        {point(0, 0, -2), vector(0.5345F, 0.8018f, 0.2673f)},
        {point(2, 0, 2), vector(0, 0, -1)},
        {point(0, 2, 2), vector(0,-1,0)},
        {point(2, 2, 0), vector(-1, 0, 0)}
    };
    for (size_t i = 0; i < rays.size(); i++) {
        const auto xs = local_intersect(c, rays[i]);
        EXPECT_EQ(xs, MISS);
    }
}


TEST(Cube, normalOnTheSurfaceOfACube) {
    const auto c = cube();
    const std::vector<Point> points{
        point(1, 0.5f, -0.8f),
        point(-1, -0.2f, 0.9f),
        point(-0.4f, 1, -0.1f),
        point(0.3f, -1, -0.7f),
        point(-0.6f, 0.3f, 1),
        point(0.4f, 0.4f, -1),
        point(1, 1, 1),
        point(-1, -1, -1)
    };
    const std::vector<Vector> normals{
        vector(1, 0, 0),
        vector(-1, 0, 0),
        vector(0, 1, 0),
        vector(0, -1, 0),
        vector(0, 0, 1),
        vector(0, 0, -1),
        vector(1, 0, 0),
        vector(-1, 0, 0)
    };
    for (size_t i = 0; i < points.size(); i++) {
        const auto p = points[i];
        const auto normal = local_normal_at(c, p);
        EXPECT_EQ(normal, normals[i]);

    }
}

TEST(Cube, IdentifyingTheFaceOfACubeFromAPoint) {
    auto p = point(-1, 0.5f, -0.25f);
    EXPECT_EQ(face_from_point(p), CubeFace::left);

    p = point(1.1f, -0.75f, 0.8f);
    EXPECT_EQ(face_from_point(p), CubeFace::right);

    p = point(0.1f, 0.6f, 0.9f);
    EXPECT_EQ(face_from_point(p), CubeFace::front);

    p = point(-0.7f, 0, -2);
    EXPECT_EQ(face_from_point(p), CubeFace::back);

    p = point(0.5f, 1, 0.9f);
    EXPECT_EQ(face_from_point(p), CubeFace::up);

    p = point(-0.2f, -1.3f, 1.1f);
    EXPECT_EQ(face_from_point(p), CubeFace::down);
}

TEST(Cube, UVMappingTheFrontFaceOfACube) {
    auto p = point(-0.5f, 0.5f, 1.0f);
    auto uv = cube_uv_front(p);
    EXPECT_EQ(uv.u, 0.25f);
    EXPECT_EQ(uv.v, 0.75f);

    p = point(0.5f, -0.5f, 1.0f);
    uv = cube_uv_front(p);
    EXPECT_EQ(uv.u, 0.75f);
    EXPECT_EQ(uv.v, 0.25f);
}

TEST(Cube, UVMappingTheBackFaceOfACube) {
    auto p = point(0.5f, 0.5f, -1.0f);
    auto uv = cube_uv_back(p);
    EXPECT_EQ(uv.u, 0.25f);
    EXPECT_EQ(uv.v, 0.75f);

    p = point(-0.5f, -0.5f, -1.0f);
    uv = cube_uv_back(p);
    EXPECT_EQ(uv.u, 0.75f);
    EXPECT_EQ(uv.v, 0.25f);
}

TEST(Cube, UVMappingTheLeftFaceOfACube) {
    auto p = point(-1, 0.5f, -0.5f);
    auto uv = cube_uv_left(p);
    EXPECT_EQ(uv.u, 0.25f);
    EXPECT_EQ(uv.v, 0.75f);

    p = point(-1, -0.5f, 0.5f);
    uv = cube_uv_left(p);
    EXPECT_EQ(uv.u, 0.75f);
    EXPECT_EQ(uv.v, 0.25f);
}

TEST(Cube, UVMappingTheRightFaceOfACube) {
    auto p = point(1, 0.5f, 0.5f);
    auto uv = cube_uv_right(p);
    EXPECT_EQ(uv.u, 0.25f);
    EXPECT_EQ(uv.v, 0.75f);

    p = point(1, -0.5f, -0.5f);
    uv = cube_uv_right(p);
    EXPECT_EQ(uv.u, 0.75f);
    EXPECT_EQ(uv.v, 0.25f);
}

TEST(Cube, UVMappingTheUpperFaceOfACube) {
    auto p = point(-0.5f, 1, -0.5f);
    auto uv = cube_uv_up(p);
    EXPECT_EQ(uv.u, 0.25f);
    EXPECT_EQ(uv.v, 0.75f);

    p = point(0.5f, 1, 0.5f);
    uv = cube_uv_up(p);
    EXPECT_EQ(uv.u, 0.75f);
    EXPECT_EQ(uv.v, 0.25f);
}

TEST(Cube, UVMappingTheLowerFaceOfACube) {
    auto p = point(-0.5f, -1, 0.5f);
    auto uv = cube_uv_down(p);
    EXPECT_EQ(uv.u, 0.25f);
    EXPECT_EQ(uv.v, 0.75f);

    p = point(0.5f, -1, -0.5f);
    uv = cube_uv_down(p);
    EXPECT_EQ(uv.u, 0.75f);
    EXPECT_EQ(uv.v, 0.25f);
}

TEST(Cube, FindingTheColorsOnAMappedCube) {
    const auto left = uv_align_check(YELLOW, CYAN, RED, BLUE, BROWN);
    const auto front = uv_align_check(CYAN, RED, YELLOW, BROWN, GREEN);
    const auto right = uv_align_check(RED, YELLOW, PURPLE, GREEN, WHITE);
    const auto back = uv_align_check(GREEN, PURPLE, CYAN, WHITE, BLUE);
    const auto up = uv_align_check(BROWN, CYAN, PURPLE, RED, YELLOW);
    const auto down = uv_align_check(PURPLE, BROWN, GREEN, BLUE, WHITE);
    const auto faces = std::vector<const AlignCheck*>({ &left, &front, &right, &back, &up, &down });
    const auto pattern = cube_map(faces);

    //left
    auto p = point(-1, 0, 0);
    auto c = YELLOW;
    EXPECT_EQ(pattern_at(pattern, p), c);
    
    p = point(-1, 0.9f, -0.9f);
    c = CYAN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-1, 0.9f, 0.9f);
    c = RED;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-1, -0.9f, -0.9f);
    c = BLUE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-1, -0.9f, 0.9f);
    c = BROWN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    //front
    p = point(0, 0, 1);
    c = CYAN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-0.9f, 0.9f, 1);
    c = RED;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(0.9f, 0.9f, 1);
    c = YELLOW;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-0.9f, -0.9f, 1);
    c = BROWN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(0.9f, -0.9f, 1);
    c = GREEN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    //right
    p = point(1, 0, 0);
    c = RED;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(1, 0.9f, 0.9f);
    c = YELLOW;
    EXPECT_EQ(pattern_at(pattern, p), c);
    
    p = point(1, 0.9f, -0.9f); 
    c = PURPLE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(1, -0.9f, 0.9f); 
    c = GREEN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(1, -0.9f, -0.9f); 
    c = WHITE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    //back 
    p = point(0, 0, -1);
    c = GREEN;
    EXPECT_EQ(pattern_at(pattern, p), c);
    
    p = point(0.9f, 0.9f, -1);
    c = PURPLE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-0.9f, 0.9f, -1);
    c = CYAN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(0.9f, -0.9f, -1); 
    c = WHITE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-0.9f, -0.9f, -1); 
    c = BLUE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    //up
    p = point(0, 1, 0); 
    c = BROWN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-0.9f, 1, -0.9f); 
    c = CYAN;

    EXPECT_EQ(pattern_at(pattern, p), c);
    p = point(0.9f, 1, -0.9f); 
    c = PURPLE;

    EXPECT_EQ(pattern_at(pattern, p), c);
    
    p = point(-0.9f, 1, 0.9f); 
    c = RED;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(0.9f, 1, 0.9f); 
    c = YELLOW;
    EXPECT_EQ(pattern_at(pattern, p), c);

    //down
    p = point(0, -1, 0); 
    c = PURPLE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-0.9f, -1, 0.9f); 
    c = BROWN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(0.9f, -1, 0.9f); 
    c = GREEN;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(-0.9f, -1, -0.9f); 
    c = BLUE;
    EXPECT_EQ(pattern_at(pattern, p), c);

    p = point(0.9f, -1, -0.9f); 
    c = WHITE;
    EXPECT_EQ(pattern_at(pattern, p), c);
}


RESTORE_WARNINGS