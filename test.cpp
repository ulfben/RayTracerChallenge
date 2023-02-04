#include "pch.h"
#include "Tuple.h"
#include "Canvas.h"
#include "Projectile.h"

#include "tests/TupleTests.h"
#include "tests/VectorTest.h"
#include "tests/ColorTests.h"


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