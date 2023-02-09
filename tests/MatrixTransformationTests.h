#pragma once
#include "../pch.h"
#include "../Matrix.h"
#include "../Tuple.h"

TEST(Transformation, translate) {    
    const Matrix4 transform = translation(5,-3,2);
    const auto p = point(-3, 4, 5);
    const auto result = transform * p;
    const auto truth = point(2, 1, 7);
    EXPECT_EQ(result, truth);    
}

TEST(Transformation, translateByInverse) {    
    const Matrix4 transform = translation(5,-3,2);
    const auto p = point(-3, 4, 5);
    const auto inv_transform = inverse(transform);
    const auto result = inv_transform * p;
    const auto truth = point(-8, 7, 3);
    EXPECT_EQ(result, truth);    
}

TEST(Transformation, translateVectorDoesNothing) {    
    const Matrix4 transform = translation(5,-3,2);
    const auto v = vector(-3, 4, 5);    
    const auto result = transform * v;    
    EXPECT_EQ(result, v);    
}