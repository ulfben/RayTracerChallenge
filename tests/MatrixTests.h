#pragma once
#include "../pch.h"
#include "../Matrix.h"

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
    Matrix3 a = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    Matrix3 b = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    Matrix2 c = { 1.0f, 2.0f, 3.0f, 4.0f };
    Matrix2 d = { 1.1f, 2.1f, 3.0f, 4.0f };
    EXPECT_FALSE(c == d);
    EXPECT_TRUE(c != d);
}

TEST(Matrix, canBeMultiplied) {
    Matrix4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2 };
    Matrix4 b{ -2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8 };
    auto product = a * b;
    const Matrix4 truth{ 20, 22, 50,  48,  44, 54, 114, 108,
                        40, 58, 110, 102, 16, 26, 46,  42 };
    EXPECT_TRUE(product == truth);
}

TEST(Matrix, canBeMultipliedWithTuple) {
    Matrix4 a{ 
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1 
    };
    auto b = Tuple{ 1, 2, 3, 1 };
    auto product = a * b; 
    const auto truth = Tuple{ 18,24,33,1 };
    EXPECT_TRUE(product == truth);
}

TEST(Matrix, hasIdentity) {
    Matrix4 a{ 
        0,1,2,4,
        1,2,4,8,
        2,4,8,16,
        4,8,16,32 
    };
    const Matrix4 id = Matrix4::identity();
    auto product = a * id;
    EXPECT_TRUE(product == a);
}

TEST(Matrix, canBeTransposed) {
    const Matrix4 a{ 
        0,9,3,0,
        9,8,0,8,
        1,8,5,3,
        0,0,5,8 
    };
    const Matrix4 truth{
        0,9,1,0,
        9,8,8,0,
        3,0,5,5,
        0,8,3,8
    };
    const Matrix4 b = transpose(a);    
    EXPECT_TRUE(b == truth);
    const Matrix4 c = transpose(b);
    EXPECT_TRUE(a == c); //2x transpose = back to original
    const Matrix4 identity = Matrix4::identity();
    EXPECT_TRUE(identity == transpose(identity));
}