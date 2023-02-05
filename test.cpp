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

constexpr Matrix2 submatrix(const Matrix3& m, Matrix3::size_type row_, Matrix3::size_type column_) noexcept {
    assert(row_ < m.rows() && column_ < m.columns() && "invalid submatrix specification. row and column must be inside the input matrix.");
    using size_type = Matrix3::size_type;
    Matrix2 r;
    size_type i = 0;    
    for (size_type k = 0; k < m.rows(); k++) {
        if (k == row_) {
            continue;
        }
        size_type j = 0;
        for (size_type l = 0; l < m.columns(); l++) {
            if (l == column_) {
                continue;
            }
            r(i, j) = m(k, l);
            j++;
        }
        i++;
    }
    return r;
}

TEST(Matrix, canCalcDeterminant) {
    const Matrix2 a{
        1,5,
        -3,2
    };
    ASSERT_EQ(determinant(a), 17);
}

TEST(Matrix, submatrixOf3x3is2x2) {
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
    EXPECT_TRUE(b == truth);
}
