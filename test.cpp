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

Real determinant(const Matrix2& m) noexcept {
    return m(0,0)*m(1,1) - m(0,1)*m(1,0);
}

TEST(Matrix, canCalcDeterminant) {
    const Matrix2 a{ 
        1,5,
        -3,2 
    };    
    ASSERT_EQ(determinant(a), 17);
}