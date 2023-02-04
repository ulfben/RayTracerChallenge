#include "pch.h"
#include "Tuple.h"
#include "Canvas.h"
#include "Projectile.h"

#include "tests/TupleTests.h"
#include "tests/VectorTest.h"
#include "tests/ColorTests.h"

template <uint8_t ROWS, uint8_t COLUMNS>
struct Matrix{
  Real data[COLUMNS][ROWS];  
};

TEST(Matrix, isRowColumnOrder) { 
    const Matrix<4, 4> m = {1.0f, 2.0f,3.0f,4.0f,
                            5.0f, 6.0f, 7.0f, 8.0f,
                            9.0f, 10.0f, 11.0f, 12.0f,
                            13.0f, 14.0f, 15.0f, 16.0f};
    EXPECT_EQ(m.data[0][0], 1.0f);
    EXPECT_EQ(m.data[0][3], 4.0f);
    EXPECT_EQ(m.data[1][0], 5.0f);
    EXPECT_EQ(m.data[1][2], 7.0f);
    EXPECT_EQ(m.data[3][3], 16.0f);    
}

TEST(Chapter2, CanOutputPPM) {
  auto c = Canvas(300, 300);
  const auto bottomEdge = static_cast<Real>(c.height());
  const auto vel = normalize(vector(1, -1.8f, 0)) * 11.25f;
  auto p = Projectile(point(0, bottomEdge, 0), vel);
  while (p.y() <= bottomEdge) {
    p.update();
    p.render(c);
  }
  std::ofstream ofs("output/chapter2.ppm", std::ofstream::out);
  ofs << c.to_ppm();
  EXPECT_TRUE(true);
}