#pragma once
#include "../pch.h"
#include "../Matrix.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Matrix, isRowMajorOrder) {
    const Matrix4 m = { 1.0f, 2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f,
                       9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };
    EXPECT_EQ(m(0, 0), 1.0f);
    EXPECT_EQ(m(0, 3), 4.0f);
    EXPECT_EQ(m(1, 0), 5.0f);
    EXPECT_EQ(m(1, 2), 7.0f);
    EXPECT_EQ(m(3, 3), 16.0f);
}

TEST(Matrix, twoByTwo) {
    const Matrix2 m = { 1.0f, 2.0f, 3.0f, 4.0f };
    EXPECT_EQ(m(0, 0), 1.0f);
    EXPECT_EQ(m(0, 1), 2.0f);
    EXPECT_EQ(m(1, 0), 3.0f);
    EXPECT_EQ(m(1, 1), 4.0f);
}

TEST(Matrix, threeByThree) {
    const Matrix3 m = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    EXPECT_EQ(m(0, 0), 1.0f);
    EXPECT_EQ(m(0, 2), 3.0f);
    EXPECT_EQ(m(1, 0), 4.0f);
    EXPECT_EQ(m(1, 2), 6.0f);
    EXPECT_EQ(m(2, 0), 7.0f);
    EXPECT_EQ(m(2, 2), 9.0f);
}

TEST(Matrix, isEqualityComparable) {
    const Matrix3 a = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    const Matrix3 b = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    EXPECT_EQ(a, b);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    const Matrix2 col = { 1.0f, 2.0f, 3.0f, 4.0f };
    const Matrix2 d = { 1.1f, 2.1f, 3.0f, 4.0f };    
    EXPECT_FALSE(col == d);
    EXPECT_TRUE(col != d);
}

TEST(Matrix, canBeMultiplied) {    
    const Matrix4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2 };
    const Matrix4 b{ -2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8 };    
    const auto product = a * b;    
    const Matrix4 expected{ 20, 22, 50,  48,  44, 54, 114, 108,
                        40, 58, 110, 102, 16, 26, 46,  42 };
    EXPECT_EQ(product, expected);
    EXPECT_TRUE(product == expected);
}

TEST(Matrix, canBeMultipliedWithTuple) {
    const Matrix4 a{ 
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1 
    };
    const auto b = point( 1, 2, 3);
    const auto product = a * b; 
    const auto expected = point( 18,24,33);
    EXPECT_TRUE(product == expected);
}

TEST(Matrix, hasIdentity) {
    const Matrix4 a{ 
        0,1,2,4,
        1,2,4,8,
        2,4,8,16,
        4,8,16,32 
    };
    const Matrix4 id = Matrix4::identity();
    const auto product = a * id;
    EXPECT_TRUE(product == a);
}

TEST(Matrix, canBeTransposed) {
    const Matrix4 a{ 
        0,9,3,0,
        9,8,0,8,
        1,8,5,3,
        0,0,5,8 
    };
    const Matrix4 expected{
        0,9,1,0,
        9,8,8,0,
        3,0,5,5,
        0,8,3,8
    };
    const Matrix4 b = transpose(a);    
    EXPECT_TRUE(b == expected);
    const Matrix4 col = transpose(b);
    EXPECT_TRUE(a == col); //2x transpose = back to original
    const Matrix4 identity = Matrix4::identity();
    EXPECT_TRUE(identity == transpose(identity));
}

TEST(Matrix, getRowFromIndex) {
    const Matrix3 a{
        1,5,0,
        -3,2,7,
        0,6,-3
    };
    EXPECT_EQ(0, index_to_row(a,0));
    EXPECT_EQ(0, index_to_row(a,2));
    EXPECT_EQ(2, index_to_row(a,6));
    EXPECT_EQ(2, index_to_row(a,8));
}

TEST(Matrix, getColumnFromIndex) {
    const Matrix3 a{
        1,5,0,
        -3,2,7,
        0,6,-3
    };
    EXPECT_EQ(0, index_to_column(a,0));
    EXPECT_EQ(1, index_to_column(a,1));
    EXPECT_EQ(2, index_to_column(a,2));
    EXPECT_EQ(0, index_to_column(a,6));
    EXPECT_EQ(1, index_to_column(a,7));
    EXPECT_EQ(2, index_to_column(a,8));
}

TEST(Matrix, canExtractSubmatrix) {
    const Matrix3 a{
        1,5,0,
        -3,2,7,
        0,6,-3
    };
    const Matrix2 expected{
        -3, 2,
        0, 6
    };
    const auto b = submatrix(a, 0, 2);
    const auto col = a.submatrix(0, 2);
    EXPECT_TRUE(b == expected);
    EXPECT_TRUE(col == expected);
}

TEST(Matrix, canCalculateMinor) {
    const Matrix3 a{
        3,5,0,
        2,-1,-7,
        6,-1,5
    };
    const auto sub = a.submatrix(1, 0);
    EXPECT_EQ(25, determinant(sub));
    EXPECT_EQ(25, minor(a, 1, 0));
}

TEST(Matrix, canCalculateCofactor) {
    const Matrix3 a{
        3,5,0,
        2,-1,-7,
        6,-1,5
    };
    EXPECT_EQ(-12, minor(a, 0, 0));
    EXPECT_EQ(-12, cofactor(a, 0, 0));
    EXPECT_EQ(25, minor(a, 1, 0));
    EXPECT_EQ(-25, cofactor(a, 1, 0));
}

TEST(Matrix, canCalculateDeterminantOn2x2) {
    const Matrix2 a{
        1,5,
        -3,2
    };
    ASSERT_EQ(determinant(a), 17);
    ASSERT_EQ(determinant2(a), 17);
}
TEST(Matrix, canCalculateDeterminantOn3x3) {
    const Matrix3 a{
        1,2,6,
        -5,8,-4,
        2,6,4
    };
    EXPECT_EQ(cofactor(a, 0, 0), 56);
    EXPECT_EQ(cofactor(a, 0, 1), 12);
    EXPECT_EQ(cofactor(a, 0, 2), -46);
    ASSERT_EQ(determinant(a), -196);
    ASSERT_EQ(determinant2(a), -196);
}
TEST(Matrix, canCalculateDeterminantOn4x4) {
    const Matrix4 a{
        -2,-8,3,5,
        -3,1,7,3,
        1,2,-9,6,
        -6,7,7,-9
    };
    EXPECT_EQ(cofactor(a, 0, 0), 690);
    EXPECT_EQ(cofactor(a, 0, 1), 447);
    EXPECT_EQ(cofactor(a, 0, 2), 210);
    EXPECT_EQ(cofactor(a, 0, 3), 51);
    ASSERT_EQ(determinant(a), -4071);
    ASSERT_EQ(determinant2(a), -4071);
}

TEST(Matrix, isInvertible) {
    const Matrix4 a = { 
        6,4,4,4,
        5,5,7,6,
        4,-9,3,-7,
        9,1,7,-6
    };
    const Matrix4 b = { 
        -4,2,-2,-3,
        9,6,2,6,
        0,-5,1,-5,
        0,0,0,0
    };
    EXPECT_TRUE(is_invertible(a));
    EXPECT_FALSE(is_invertible(b));    
}

TEST(Matrix, canInvert) {
    const Matrix4 a = { 
        -5,2,6,-8,
        1,-5,1,8,
        7,7,-6,-7,
        1,-3,7,4
    };        
    const Matrix4 expected{ 
        0.218045f, 0.451128f, 0.240602f, -0.0451128f, 
        -0.808271f, -1.45677f, -0.443609f, 0.520677f, 
        -0.0789474f, -0.223684f, -0.0526316f, 0.197368f, 
        -0.522556f, -0.81391f, -0.300752f, 0.306391f 
    };    
    const Matrix4 BOOK_ORACLE{ //expected / "good enough" according to the book.
         0.21805f, 0.45113f,0.24060f,-0.04511f,
        -0.80827f,-1.45677f,-0.44361f,0.52068f,
        -0.07895f,-0.22368f,-0.05263f,0.19737f,
        -0.52256f,-0.81391f,-0.30075f,0.30639f
    };
    const Matrix4 inv_a = inverse(a);

    EXPECT_TRUE(is_invertible(a));    
    EXPECT_EQ(determinant(a), 532);
    EXPECT_FLOAT_EQ(cofactor(a, 2, 3), -160.0f);    
    EXPECT_FLOAT_EQ(inv_a(3,2), -160.0f/532);
    EXPECT_FLOAT_EQ(cofactor(a, 3, 2), 105.0f);
    EXPECT_FLOAT_EQ(inv_a(2,3), 105.0f/532);
    EXPECT_TRUE(inv_a == expected);    
    EXPECT_EQ(inv_a, expected);    
}

TEST(Matrix, canInvert2) {
    const Matrix4 a = { 
        8,-5,9,2,
        7,5,6,1,
        -6,0,9,6,
        -3,0,-9,-4
    };        
    const Matrix4 expected{
        -0.153846f, -0.153846f, -0.282051f, -0.538462f,
        -0.0769231f, 0.123077f, 0.025641f, 0.0307692f,
        0.358974f, 0.358974f, 0.435897f, 0.923077f,
        -0.692308f, -0.692308f, -0.769231f, -1.92308f
    };
    const Matrix4 BOOK_ORACLE{ //expected / "good enough" according to the book.
        -0.15385f, -0.15385f, -0.28205f, -0.53846f,
        -0.07692f, 0.12308f, 0.02564f, 0.03077f,
        0.35897f, 0.35897f, 0.43590f, 0.92308f,
        -0.69231f, -0.69231f, -0.76923f, -1.92308f
    };
    const Matrix4 inv_a = inverse(a);
        
    EXPECT_EQ(inv_a, expected);    
}

TEST(Matrix, canInvert3) {
    const Matrix4 a = { 
        9,3,0,9,
        -5,-2,-6,-3,
        -4,9,6,4,
        -7,6,6,2
    };        
    const Matrix4 expected{
        -0.0407407f, -0.0777778f, 0.144444f, -0.222222f,
        -0.0777778f, 0.0333333f, 0.366667f, -0.333333f,
        -0.0290123f, -0.146296f, -0.109259f, 0.12963f,
        0.177778f, 0.0666667f, -0.266667f, 0.333333f
    };
    const Matrix4 BOOK_ORACLE{ //expected / "good enough" according to the book.
        -0.04074f, -0.07778f, 0.14444f, -0.22222f,
        -0.07778f, 0.03333f, 0.36667f, -0.33333f,
        -0.02901f, -0.14630f, -0.10926f, 0.12963f,
        0.17778f, 0.06667f, -0.26667f, 0.33333f
    };
    const Matrix4 inv_a = inverse(a);
        
    EXPECT_EQ(inv_a, expected);    
}

TEST(Matrix, canRoundtripInvert) {
    const Matrix4 a = { 
        3,-9,7,3,
        3,-8,2,-9,
        -4,4,4,1,
        -6,5,-1,1
    };        
    const Matrix4 b{ 
        8,2,2,2,
        3,-8,2,-9,
        -4,4,4,1,
        -6,5,-1,1
    };    
    const Matrix4 prod = a * b;
    const Matrix4 inv_b = inverse(b);          
    EXPECT_EQ(prod*inv_b, a);    
}

TEST(Matrix, RodriguesRotationAroundZAxis) {
    //90 degrees counter-clockwise around z-axis
     const auto axis = vector(0, 0, 1);
    const auto t = 90 * math::TO_RAD;
    const auto m = rotation(axis, t); 
    
    const auto result1 = m * vector(1, 0, 0);
    EXPECT_EQ(result1, vector(0,1,0));  

    const auto result2 = m * vector(0, 1, 0);
    EXPECT_EQ(result2, vector(-1,0,0));        
}

TEST(Matrix, RodriguesRotationAroundXAxis) {
    //checks that a rotation of 90 degrees around the x-axis correctly transforms the y-axis to the z-axis. 
    const auto axis = vector(1, 0, 0);
    const auto t = 90 * math::TO_RAD;
    const auto R = rotation(axis, t);    
    const auto in = vector(0, 1, 0);
    const auto out = R * in;
    const auto expected =  vector(0, 0, 1);
    EXPECT_EQ(out, expected);
}

TEST(Matrix, RodriguesRotationAroundZAxisTwoComponents) {
    //checks that a rotation of 45 degrees around the z-axis correctly transforms a vector lying in the xy-plane.
    using std::sin, std::cos;
    const auto axis = vector(0, 0, 1);
    const auto t = 45 * math::TO_RAD;
    const auto R = rotation(axis, t);
    const auto in = vector(1, 1, 0);
    const auto out = R * in;
    const auto expected = vector(cos(t)-sin(t), sin(t)+cos(t), 0);
    EXPECT_EQ(out, expected);
}

TEST(Matrix, RodriguesIdentityRotation) {
    // checks that the rotation function returns the identity matrix when given the same input vector twice, indicating that no rotation is needed.
    const auto u = vector(1, 0, 0);
    const auto v = vector(1, 0, 0);
    const auto R = rotation(u, v);
    EXPECT_EQ(R, Matrix4Identity);
}

TEST(Matrix, RodriguesYAxisRotation) {
    // rotates a vector lying in the xy-plane (in) by 90 degrees around the z-axis, when given the x-axis and y-axis as input vectors (cross u v)
    const auto u = vector(1, 0, 0);
    const auto v = vector(0, 1, 0);
    const auto R = rotation(u, v);
    const auto in = vector(1, 1, 0);
    const auto out = R * in;
    EXPECT_EQ(out, vector(-1, 1, 0));
}

RESTORE_WARNINGS