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

TEST(Transformation, inverseTranslate) {    
    const Matrix4 inv_transform = inverse(translation(5,-3,2));
    const auto p = point(-3, 4, 5);    
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

TEST(Transformation, scaling) {    
    const Matrix4 transform = scaling(2,3,4);
    const auto v = vector(-4, 6, 8);    
    const auto result = transform * v;    
    const auto truth = vector(-8, 18, 32);
    EXPECT_EQ(result, truth);    
}

TEST(Transformation, inverseScaling) {        
    const Matrix4 inv_tranform = inverse(scaling(2,3,4));
    const auto v = vector(-4, 6, 8);    
    const auto result = inv_tranform * v;    
    const auto truth = vector(-2, 2, 2);
    EXPECT_EQ(result, truth);    
}

TEST(Transformation, reflectionByScaling) {        
    const Matrix4 transform = scaling(-1,1,1);
    const auto p = point(2, 3, 4);    
    const auto result = transform * p;    
    const auto truth = point(-2, 3, 4);
    EXPECT_EQ(result, truth);    
}