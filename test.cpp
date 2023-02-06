#include "pch.h"
#include "Canvas.h"
#include "Projectile.h"
#include "Tuple.h"
#include "Matrix.h"

#include "tests/ColorTests.h"
#include "tests/TupleTests.h"
#include "tests/VectorTests.h"
#include "tests/MatrixTests.h"

//TEST(Chapter2, CanOutputPPM) {
//    auto c = Canvas(300, 300);
//    const auto bottomEdge = static_cast<Real>(c.height());
//    const auto vel = normalize(vector(1, -1.8f, 0)) * 11.25f;
//    auto p = Projectile(point(0, bottomEdge, 0), vel);
//    while (p.y() <= bottomEdge) {
//        p.update();
//        p.render(c);
//    }
//    std::ofstream ofs("output/chapter2.ppm", std::ofstream::out);
//    ofs << c.to_ppm();
//    EXPECT_TRUE(true);
//}

constexpr Real determinant(const Matrix2& m) noexcept {
    return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
}

template <uint8_t ROWS, uint8_t COLUMNS>
constexpr auto submatrix(const Matrix<ROWS, COLUMNS>& in, uint8_t remove_row, uint8_t remove_column) noexcept {
    assert(remove_row < ROWS && remove_column < COLUMNS && "invalid submatrix specification. row and column must be inside the input matrix.");   
    using size_type = Matrix<ROWS, COLUMNS>::size_type;  
    Matrix<ROWS - 1, COLUMNS - 1> out;          
    size_type ri = 0;
    for (size_type i = 0; i < ROWS; i++) {
        if (i == remove_row) { continue; }
        for (size_type j = 0; j < COLUMNS; j++) {
            if (j == remove_column) { continue; }
            out[ri++] = in[i * COLUMNS + j];
        }
    }     
    return out;
}

TEST(Matrix, canCalcDeterminant) {
    const Matrix2 a{
        1,5,
        -3,2
    };
    ASSERT_EQ(determinant(a), 17);
}
TEST(Matrix, getRowFromIndex) {
    const Matrix3 a{
        1,5,0,
        -3,2,7,
        0,6,-3
    };    
    EXPECT_EQ(0, a.index_to_row(0)); 
    EXPECT_EQ(0, a.index_to_row(2)); 
    EXPECT_EQ(2, a.index_to_row(6)); 
    EXPECT_EQ(2, a.index_to_row(8));     
}

TEST(Matrix, getColumnFromIndex) {
    const Matrix3 a{
        1,5,0,
        -3,2,7,
        0,6,-3
    };    
    EXPECT_EQ(0, a.index_to_column(0)); 
    EXPECT_EQ(1, a.index_to_column(1)); 
    EXPECT_EQ(2, a.index_to_column(2)); 
    EXPECT_EQ(0, a.index_to_column(6)); 
    EXPECT_EQ(1, a.index_to_column(7)); 
    EXPECT_EQ(2, a.index_to_column(8)); 
}

TEST(Matrix, canExtractSubmatrix) {
    const Matrix3 a{
        1,5,0,
        -3,2,7,
        0,6,-3
    };
    const Matrix2 truth{
        -3, 2,
        0, 6
    };
    const auto b = submatrix(a, 0, 2);
    const auto c = a.submatrix(0, 2);
    EXPECT_TRUE(b == truth);
    EXPECT_TRUE(c == truth);
}

