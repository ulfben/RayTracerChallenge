#include "pch.h"
#include "Canvas.h"
#include "Projectile.h"
#include "Tuple.h"


#include "tests/ColorTests.h"
#include "tests/TupleTests.h"
#include "tests/VectorTest.h"

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

template <uint8_t ROWS, uint8_t COLUMNS> struct Matrix {
    static_assert((uint16_t(ROWS)* COLUMNS) < std::numeric_limits<uint8_t>::max(),
        "Matrix is limited to 255 elements.");
    using value_type = Real;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = uint8_t;
    using reference = value_type&;
    using const_reference = const value_type&;


    value_type _data[COLUMNS * ROWS]{};

    constexpr reference operator()(size_type row, size_type col) noexcept {
        return _data[COLUMNS * row + col];
    }
    constexpr value_type operator()(size_type row, size_type col) const noexcept {
        return _data[COLUMNS * row + col];
    }

    constexpr size_type columns() const noexcept { return COLUMNS; }
    constexpr size_type rows() const noexcept { return ROWS; }
    constexpr pointer data() noexcept { return &_data[0]; }
    constexpr const_pointer data() const noexcept { return &_data[0]; }
    constexpr size_type size() const noexcept { return COLUMNS * ROWS; }

    constexpr iterator begin() noexcept { return data(); }
    constexpr iterator end() noexcept { return begin() + size(); }
    constexpr const_iterator begin() const noexcept { return data(); }
    constexpr const_iterator end() const noexcept { return begin() + size(); }

    static constexpr Matrix<ROWS, COLUMNS> identity() noexcept {
        static_assert(ROWS == COLUMNS, "Matrix::identity only supports square matrixes");
        Matrix<ROWS, COLUMNS> result;
        for (size_type i = 0; i < COLUMNS; ++i) {
            result(i, i) = 1;
        }
        return result;
    }
};

using Matrix4 = Matrix<4, 4>;
using Matrix3 = Matrix<3, 3>;
using Matrix2 = Matrix<2, 2>;

template <uint8_t ROWS, uint8_t COLUMNS>
constexpr bool operator==(const Matrix<ROWS, COLUMNS>& lhs,
    const Matrix<ROWS, COLUMNS>& rhs) noexcept {
    using std::ranges::equal;
    return equal(lhs, rhs,
        [](auto a, auto b) { return math::almost_equal(a, b); });
}

template <uint8_t ROWS, uint8_t COLUMNS>
Matrix<ROWS, COLUMNS> operator*(const Matrix<ROWS, COLUMNS>& lhs,
    const Matrix<ROWS, COLUMNS>& rhs) noexcept {
    Matrix<ROWS, COLUMNS> result;
    for (uint8_t row = 0; row < ROWS; ++row) {
        for (uint8_t col = 0; col < COLUMNS; ++col) {
            for (uint8_t i = 0; i < COLUMNS; ++i) {
                result(row, col) += lhs(row, i) * rhs(i, col);
            }
        }
    }
    return result;
}

Tuple operator*(const Matrix4& lhs, const Tuple rhs) noexcept {
   const auto x = rhs.x*lhs(0,0) + rhs.y*lhs(0,1) + rhs.z*lhs(0,2) + rhs.w*lhs(0,3);
   const auto y = rhs.x*lhs(1,0) + rhs.y*lhs(1,1) + rhs.z*lhs(1,2) + rhs.w*lhs(1,3);
   const auto z = rhs.x*lhs(2,0) + rhs.y*lhs(2,1) + rhs.z*lhs(2,2) + rhs.w*lhs(2,3);
   const auto w = rhs.x*lhs(3,0) + rhs.y*lhs(3,1) + rhs.z*lhs(3,2) + rhs.w*lhs(3,3);   
   return Tuple{x, y, z, w};
}

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
    Matrix4 a{ 1, 2, 3, 4,
               2, 4, 4, 2,
                8,6,4,1,
                0,0,0,1 };
    auto b = Tuple{ 1, 2, 3, 1 };
    auto product = a * b; 
    const auto truth = Tuple{ 18,24,33,1 };
    EXPECT_TRUE(product == truth);
}

TEST(Matrix, hasIdentity) {
    Matrix4 a{ 0,1,2,4,
                1,2,4,8,
                2,4,8,16,
                4,8,16,32 };
    const Matrix4 id = Matrix4::identity();
    auto product = a * id;
    EXPECT_TRUE(product == a);
}