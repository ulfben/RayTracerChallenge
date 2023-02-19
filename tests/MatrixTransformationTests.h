#pragma once
#include "../pch.h"
#include "../Matrix.h"
#include "../Tuple.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(TransformationMatrix, translation) {    
    const Matrix4 transform = translation(5,-3,2);
    const auto p = point(-3, 4, 5);
    const auto result = transform * p;
    const auto expected = point(2, 1, 7);
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, inverseTranslation) {    
    const Matrix4 inv_transform = inverse(translation(5,-3,2));
    const auto p = point(-3, 4, 5);    
    const auto result = inv_transform * p;
    const auto expected = point(-8, 7, 3);
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, translatingVectorDoesNothing) {    
    const Matrix4 transform = translation(5,-3,2);
    const auto v = vector(-3, 4, 5);    
    const auto result = transform * v;    
    EXPECT_EQ(result, v);    
}

TEST(TransformationMatrix, scaling) {    
    const Matrix4 transform = scaling(2,3,4);
    const auto v = vector(-4, 6, 8);    
    const auto result = transform * v;    
    const auto expected = vector(-8, 18, 32);
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, inverseScaling) {        
    const Matrix4 inv_tranform = inverse(scaling(2,3,4));
    const auto v = vector(-4, 6, 8);    
    const auto result = inv_tranform * v;    
    const auto expected = vector(-2, 2, 2);
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, reflectionByScaling) {        
    const Matrix4 transform = scaling(-1,1,1);
    const auto p = point(2, 3, 4);    
    const auto result = transform * p;    
    const auto expected = point(-2, 3, 4);
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, rotationOnX) {       
    constexpr auto ROOT_TWO = math::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto half_quarter = rotation_x(math::PI * 0.25f);
    const auto full_quarter = rotation_x(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(0.0f, ROOT_TWO / 2, ROOT_TWO / 2));       

    const auto result = full_quarter * p;
    const auto expected = point(0.0f,0.0f,1.0f);
    EXPECT_EQ(result, expected);   
}

TEST(TransformationMatrix, invRotationOnX) {         
    constexpr auto ROOT_TWO = math::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto inv_half_quarter = inverse(rotation_x(math::PI * 0.25f));    
    EXPECT_EQ(inv_half_quarter*p, point(0.0f, ROOT_TWO / 2, -ROOT_TWO / 2));        
}

TEST(TransformationMatrix, rotationOnY) {       
    constexpr auto ROOT_TWO = math::sqrt(2.0f); 
    const auto p = point(0.0f, 0.0f, 1.0f);    
    const auto half_quarter = rotation_y(math::PI * 0.25f);
    const auto full_quarter = rotation_y(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(ROOT_TWO / 2, 0.0f, ROOT_TWO / 2));

    const auto result = full_quarter * p;
    const auto expected = point(1.0f,0.0f,0.0f);
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, rotationOnZ) {       
    constexpr auto ROOT_TWO = math::sqrt(2.0f); 
    const auto p = point(0.0f, 1.0f, 0.0f);    
    const auto half_quarter = rotation_z(math::PI * 0.25f);
    const auto full_quarter = rotation_z(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(-ROOT_TWO / 2, ROOT_TWO / 2, 0.0f));    

    const auto result = full_quarter * p;
    const auto expected = point(-1.0f, 0.0f, 0.0f); //actual y:-0.0000000437114
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, shearingMovesXInProportionToY) {               
    const auto transform = shearing(1,0,0,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(5,3,4));    
}
TEST(TransformationMatrix, shearingMovesXInProportionToZ) {               
    const auto transform = shearing(0,1,0,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(6,3,4));    
}
TEST(TransformationMatrix, shearingMovesYInProportionToX) {               
    const auto transform = shearing(0,0,1,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,5,4));    
}
TEST(TransformationMatrix, shearingMovesYInProportionToZ) {               
    const auto transform = shearing(0,0,0,1,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,7,4));    
}
TEST(TransformationMatrix, shearingMovesZInProportionToX) {               
    const auto transform = shearing(0,0,0,0,1,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,3,6));    
}
TEST(TransformationMatrix, shearingMovesZInProportionToY) {               
    const auto transform = shearing(0,0,0,0,0,1);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,3,7));    
}

TEST(TransformationMatrix, individualTransformsAreAppliedInSequence) {           
    const auto p = point(1, 0, 1);    
    const auto rot = rotation_x(math::HALF_PI);    
    const auto scal = scaling(5, 5, 5);    
    const auto tran = translation(10, 5, 7);

    const auto p2 = rot * p; //apply rotation first    
    auto expected = point(1.0f, -1.0f, 0.0f);
    EXPECT_EQ(p2, expected);    

    const auto p3 = scal * p2; //then apply scaling
    expected = point(5.0f, -5.0f, 0.0f);
    EXPECT_EQ(p3, expected);    

    const auto result = tran * p3; //then apply translation
    expected = point(15.0f, 0.0f, 7.0f);
    EXPECT_EQ(result, expected);    
}

TEST(TransformationMatrix, chainedTransformsAreAppliedInReverse) {           
    const auto p = point(1, 0, 1);    
    const auto rot = rotation_x(math::HALF_PI);    
    const auto scal = scaling(5, 5, 5);    
    const auto tran = translation(10, 5, 7);

    const auto transform = tran * scal * rot;

    const auto result = transform * p; //then apply translation
    EXPECT_EQ(result, point(15.0f, 0.0f, 7.0f));    
}
TEST(RotationMatrix, aroundX) {
    constexpr auto radians = math::PI / 4.0f;
    const auto result = rotation(radians, 0.0f, 0.0f);
    const auto expected = rotation_x(radians);
    EXPECT_EQ(result, expected);    
}

TEST(RotationMatrix, aroundY) {
    constexpr auto radians = math::PI / 4.0f;
    const auto result = rotation(0.0f, radians, 0.0f);
    const auto expected = rotation_y(radians);
    EXPECT_EQ(result, expected);    
}

TEST(RotationMatrix, aroundZ) {
    constexpr auto radians = math::PI / 4.0f;
    const auto result = rotation(0.0f, 0.0f, radians);
    const auto expected = rotation_z(radians);
    EXPECT_EQ(result, expected);    
}

TEST(RotationMatrix, aroundAllAxis) {
    constexpr auto radians_x = math::PI / 4.0f;
    constexpr auto radians_y = math::PI / 3.0f;
    constexpr auto radians_z = math::PI / 6.0f;
    const auto result = rotation(radians_x, radians_y, radians_z);    
    const auto expected = rotation_x(radians_x) * rotation_y(radians_y) * rotation_z(radians_z);
    EXPECT_EQ(result, expected);    
}

RESTORE_WARNINGS