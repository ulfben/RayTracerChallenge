#include "pch.h"
#include "Canvas.h"
#include "Projectile.h"
#include "Tuple.h"

#include "tests/ColorTests.h"
#include "tests/TupleTests.h"
#include "tests/VectorTest.h"

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

template <uint8_t ROWS, uint8_t COLUMNS> 
struct Matrix { 
  static_assert((uint16_t(ROWS) * COLUMNS) < std::numeric_limits<uint8_t>::max(), "Matrix is limited to 255 elements.");
  using value_type = Real;
  using pointer = value_type *;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using size_type = uint8_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  
  value_type _data[COLUMNS * ROWS]{};

  constexpr const_reference operator()(size_type row, size_type col) noexcept {
    return _data[COLUMNS * row + col];
  }
  constexpr value_type operator()(size_type row, size_type col) const noexcept {
    return _data[COLUMNS * row + col];
  }

  constexpr pointer data() noexcept { return &_data[0]; }
  constexpr const_pointer data() const noexcept {
      return &_data[0];
  } 
  constexpr size_type size() const noexcept { return COLUMNS * ROWS; }
  
  constexpr iterator begin() noexcept { return data(); }
  constexpr iterator end() noexcept { return begin() + size(); }
  constexpr const_iterator begin() const noexcept { return data(); }
  constexpr const_iterator end() const noexcept { return begin() + size(); }
};

template <uint8_t ROWS, uint8_t COLUMNS>
constexpr bool operator==(Matrix<ROWS, COLUMNS> &lhs,
                          Matrix<ROWS, COLUMNS> &rhs) noexcept {
  using std::ranges::equal;
  return equal(lhs, rhs, [](auto a, auto b) { return math::almost_equal(a, b); });
}
using Matrix4 = Matrix<4, 4>;
using Matrix3 = Matrix<3, 3>;
using Matrix2 = Matrix<2, 2>;

TEST(Matrix, isRowColumnOrder) {
  const Matrix4 m = {1.0f, 2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f,
                     9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
  EXPECT_EQ(m(0, 0), 1.0f);
  EXPECT_EQ(m(0, 3), 4.0f);
  EXPECT_EQ(m(1, 0), 5.0f);
  EXPECT_EQ(m(1, 2), 7.0f);
  EXPECT_EQ(m(3, 3), 16.0f);
}

TEST(Matrix, twoByTwo) {
  const Matrix2 m = {1.0f, 2.0f, 3.0f, 4.0f};
  EXPECT_EQ(m(0, 0), 1.0f);
  EXPECT_EQ(m(0, 1), 2.0f);
  EXPECT_EQ(m(1, 0), 3.0f);
  EXPECT_EQ(m(1, 1), 4.0f);
}

TEST(Matrix, threeByThree) {
  const Matrix3 m = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  EXPECT_EQ(m(0, 0), 1.0f);
  EXPECT_EQ(m(0, 2), 3.0f);
  EXPECT_EQ(m(1, 0), 4.0f);
  EXPECT_EQ(m(1, 2), 6.0f);
  EXPECT_EQ(m(2, 0), 7.0f);
  EXPECT_EQ(m(2, 2), 9.0f);
}

TEST(Matrix, isEqualityComparable) {
  Matrix3 a = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  Matrix3 b = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
}

TEST(Matrix, isEqualityComparable2) {
  Matrix2 a = {1.0f, 2.0f, 3.0f, 4.0f};
  Matrix2 b = {1.1f, 2.1f, 3.0f, 4.0f};
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}