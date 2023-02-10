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

TEST(Transformation, rotationOnX) {       
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto half_quarter = rotation_x(math::PI * 0.25f);
    const auto full_quarter = rotation_x(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(0.0f, ROOT_TWO / 2, ROOT_TWO / 2));    
    EXPECT_EQ(full_quarter*p, point(0.0f,0.0f,1.0f));    
}

TEST(Transformation, invRotationOnX) {         
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto inv_half_quarter = inverse(rotation_x(math::PI * 0.25f));    
    EXPECT_EQ(inv_half_quarter*p, point(0.0f, ROOT_TWO / 2, -ROOT_TWO / 2));        
}

TEST(Transformation, rotationOnY) {       
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 0, 1);    
    const auto half_quarter = rotation_y(math::PI * 0.25f);
    const auto full_quarter = rotation_y(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(ROOT_TWO / 2, 0.0f, ROOT_TWO / 2));    
    EXPECT_EQ(full_quarter*p, point(1.0f,0.0f,0.0f));    
}

TEST(Transformation, rotationOnZ) {       
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto half_quarter = rotation_z(math::PI * 0.25f);
    const auto full_quarter = rotation_z(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(-ROOT_TWO / 2, ROOT_TWO / 2, 0.0f));    
    EXPECT_EQ(full_quarter*p, point(-1.0f,0.0f,0.0f));    
}

TEST(Shearing, movesXInProportionToY) {               
    const auto transform = shearing(1,0,0,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(5,3,4));    
}
TEST(Shearing, movesXInProportionToZ) {               
    const auto transform = shearing(0,1,0,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(6,3,4));    
}
TEST(Shearing, movesYInProportionToX) {               
    const auto transform = shearing(0,0,1,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,5,4));    
}
TEST(Shearing, movesYInProportionToZ) {               
    const auto transform = shearing(0,0,0,1,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,7,4));    
}
TEST(Shearing, movesZInProportionToX) {               
    const auto transform = shearing(0,0,0,0,1,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,3,6));    
}
TEST(Shearing, movesZInProportionToY) {               
    const auto transform = shearing(0,0,0,0,0,1);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,3,7));    
}
