#pragma once
#include "../pch.h"
#include "../Matrix.h"
#include "../Tuple.h"

TEST(Tranformations, translate) {    
    const Matrix4 transform = translation(5,-3,2);
    const auto p = point(-3, 4, 5);
    const auto result = transform * p;
    const auto truth = point(2, 1, 7);
    EXPECT_EQ(result, truth);    
}

TEST(Tranformations, inverseTranslate) {    
    const Matrix4 inv_transform = inverse(translation(5,-3,2));
    const auto p = point(-3, 4, 5);    
    const auto result = inv_transform * p;
    const auto truth = point(-8, 7, 3);
    EXPECT_EQ(result, truth);    
}

TEST(Tranformations, translateVectorDoesNothing) {    
    const Matrix4 transform = translation(5,-3,2);
    const auto v = vector(-3, 4, 5);    
    const auto result = transform * v;    
    EXPECT_EQ(result, v);    
}

TEST(Tranformations, scaling) {    
    const Matrix4 transform = scaling(2,3,4);
    const auto v = vector(-4, 6, 8);    
    const auto result = transform * v;    
    const auto truth = vector(-8, 18, 32);
    EXPECT_EQ(result, truth);    
}

TEST(Tranformations, inverseScaling) {        
    const Matrix4 inv_tranform = inverse(scaling(2,3,4));
    const auto v = vector(-4, 6, 8);    
    const auto result = inv_tranform * v;    
    const auto truth = vector(-2, 2, 2);
    EXPECT_EQ(result, truth);    
}

TEST(Tranformations, reflectionByScaling) {        
    const Matrix4 transform = scaling(-1,1,1);
    const auto p = point(2, 3, 4);    
    const auto result = transform * p;    
    const auto truth = point(-2, 3, 4);
    EXPECT_EQ(result, truth);    
}

TEST(Tranformations, rotationOnX) {       
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto half_quarter = rotation_x(math::PI * 0.25f);
    const auto full_quarter = rotation_x(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(0.0f, ROOT_TWO / 2, ROOT_TWO / 2));    
    EXPECT_EQ(full_quarter*p, point(0.0f,0.0f,1.0f));    
}

TEST(Tranformations, invRotationOnX) {         
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto inv_half_quarter = inverse(rotation_x(math::PI * 0.25f));    
    EXPECT_EQ(inv_half_quarter*p, point(0.0f, ROOT_TWO / 2, -ROOT_TWO / 2));        
}

TEST(Tranformations, rotationOnY) {       
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 0, 1);    
    const auto half_quarter = rotation_y(math::PI * 0.25f);
    const auto full_quarter = rotation_y(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(ROOT_TWO / 2, 0.0f, ROOT_TWO / 2));    
    EXPECT_EQ(full_quarter*p, point(1.0f,0.0f,0.0f));    
}

TEST(Tranformations, rotationOnZ) {       
    const auto ROOT_TWO = std::sqrt(2.0f); 
    const auto p = point(0, 1, 0);    
    const auto half_quarter = rotation_z(math::PI * 0.25f);
    const auto full_quarter = rotation_z(math::PI * 0.5f);    
    EXPECT_EQ(half_quarter*p, point(-ROOT_TWO / 2, ROOT_TWO / 2, 0.0f));    
    EXPECT_EQ(full_quarter*p, point(-1.0f,0.0f,0.0f));    
}

TEST(Tranformations, shearingMovesXInProportionToY) {               
    const auto transform = shearing(1,0,0,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(5,3,4));    
}
TEST(Tranformations, shearingMovesXInProportionToZ) {               
    const auto transform = shearing(0,1,0,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(6,3,4));    
}
TEST(Tranformations, shearingMovesYInProportionToX) {               
    const auto transform = shearing(0,0,1,0,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,5,4));    
}
TEST(Tranformations, shearingMovesYInProportionToZ) {               
    const auto transform = shearing(0,0,0,1,0,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,7,4));    
}
TEST(Tranformations, shearingMovesZInProportionToX) {               
    const auto transform = shearing(0,0,0,0,1,0);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,3,6));    
}
TEST(Tranformations, shearingMovesZInProportionToY) {               
    const auto transform = shearing(0,0,0,0,0,1);
    const auto p = point(2, 3, 4);    
    EXPECT_EQ(transform*p, point(2,3,7));    
}

TEST(Tranformations, individualTransformsAreAppliedInSequence) {           
    const auto p = point(1, 0, 1);    
    const auto rot = rotation_x(math::HALF_PI);    
    const auto scal = scaling(5, 5, 5);    
    const auto tran = translation(10, 5, 7);

    const auto p2 = rot * p; //apply rotation first    
    EXPECT_EQ(p2, point(1.0f, -1.0f, 0.0f));    

    const auto p3 = scal * p2; //then apply scaling
    EXPECT_EQ(p3, point(5.0f, -5.0f, 0.0f));    

    const auto result = tran * p3; //then apply translation
    EXPECT_EQ(result, point(15.0f, 0.0f, 7.0f));    
}

TEST(Tranformations, chainedTransformsAreAppliedInReverse) {           
    const auto p = point(1, 0, 1);    
    const auto rot = rotation_x(math::HALF_PI);    
    const auto scal = scaling(5, 5, 5);    
    const auto tran = translation(10, 5, 7);

    const auto transform = tran * scal * rot;

    const auto result = transform * p; //then apply translation
    EXPECT_EQ(result, point(15.0f, 0.0f, 7.0f));    
}
TEST(RotationTest, RotationAroundX) {
    const auto radians = math::PI / 4.0f;
    const auto result = rotation(radians, 0.0f, 0.0f);
    const auto expected = rotation_x(radians);
    EXPECT_EQ(result, expected);    
}

TEST(RotationTest, RotationAroundY) {
    const auto radians = math::PI / 4.0f;
    const auto result = rotation(0.0f, radians, 0.0f);
    const auto expected = rotation_y(radians);
    EXPECT_EQ(result, expected);    
}

TEST(RotationTest, RotationAroundZ) {
    const auto radians = math::PI / 4.0f;
    const auto result = rotation(0.0f, 0.0f, radians);
    const auto expected = rotation_z(radians);
    EXPECT_EQ(result, expected);    
}

TEST(RotationTest, RotationAroundAllAxis) {
    const auto radians_x = math::PI / 4.0f;
    const auto radians_y = math::PI / 3.0f;
    const auto radians_z = math::PI / 6.0f;
    const auto result = rotation(radians_x, radians_y, radians_z);    
    const auto expected = rotation_x(radians_x) * rotation_y(radians_y) * rotation_z(radians_z);
    EXPECT_EQ(result, expected);    
}
