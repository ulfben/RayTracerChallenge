#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Math.h"
template <uint8_t ROWS_, uint8_t COLUMNS_> 
struct Matrix final {
    static_assert(ROWS_ > 0 && COLUMNS_ > 0 && "Matrix dimensions must be non-zero.");
    static_assert((uint16_t(ROWS_) * COLUMNS_) < std::numeric_limits<uint8_t>::max(), "Matrix is limited to 255 elements.");
    using value_type = Real;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = uint8_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using submatrix_type = Matrix<ROWS_ - 1, COLUMNS_ - 1>;
    static constexpr size_type ROWS = ROWS_;
    static constexpr size_type COLUMNS = COLUMNS_;

    value_type _data[COLUMNS_ * ROWS_]{};

    constexpr reference operator()(size_type row, size_type col) noexcept {
        assert(row < rows() && col < columns() && "Matrix::operator(row,col) arguments are out of bounds");
        return _data[columns() * row + col];
    }
    constexpr value_type operator()(size_type row, size_type col) const noexcept {
        assert(row < rows() && col < columns() && "Matrix::operator(row,col) arguments are out of bounds");
        return _data[columns() * row + col];
    }
    constexpr value_type operator[](size_type i) const noexcept {
        assert(i < size() && "Matrix::operator[i] index is out of bounds");
        return _data[i];
    }
    constexpr reference operator[](size_type i) noexcept {
        assert(i < size() && "Matrix::operator[i] index is out of bounds");
        return _data[i];
    }
    constexpr reference operator()(size_type i) noexcept {
        assert(i < size() && "Matrix::operator(i) index is out of bounds");
        return _data[i];
    }
    constexpr value_type operator()(size_type i) const noexcept {
        assert(i < size() && "Matrix::operator(i) index is out of bounds");
        return _data[i];
    }
        
    constexpr auto submatrix(size_type remove_row, size_type remove_column) const noexcept {
        assert(remove_row < rows() && remove_column < columns() && "Matrix::submatrix() arguments are out of range. row and column must be inside the matrix.");
        submatrix_type r;          
        size_type ri = 0;
        for (size_type i = 0; i < size(); i++) {
            if(index_to_row(i) == remove_row) { i += columns(); } //skip the entire row
            if(index_to_column(i) != remove_column) {
                r[ri++] = _data[i];
            }
        }     
        return r;
    }

    constexpr size_type columns() const noexcept { return COLUMNS_; }
    constexpr size_type rows() const noexcept { return ROWS_; }
    constexpr pointer data() noexcept { return &_data[0]; }
    constexpr const_pointer data() const noexcept { return &_data[0]; }
    constexpr size_type size() const noexcept { return columns() * rows(); }

    constexpr iterator begin() noexcept { return data(); }
    constexpr iterator end() noexcept { return begin() + size(); }
    constexpr const_iterator begin() const noexcept { return data(); }
    constexpr const_iterator end() const noexcept { return begin() + size(); }

     constexpr size_type index_to_column(size_type index) const noexcept {
        assert(index < size());
        return index % columns();
    }
    constexpr size_type index_to_row(size_type index) const noexcept {
        assert(index < size());
        return index / columns();
    }

    static constexpr auto identity() noexcept {
        static_assert(ROWS_ == COLUMNS_, "Matrix::identity only supports square matrixes");
        Matrix<ROWS_, COLUMNS_> result;
        for (size_type i = 0; i < COLUMNS_; ++i) {
            result(i, i) = 1;
        }
        return result;
    }
};

using Matrix4 = Matrix<4, 4>;
using Matrix3 = Matrix<3, 3>;
using Matrix2 = Matrix<2, 2>;
using Matrix1 = Matrix<1, 1>; //to stop template deductions from breaking
static constexpr auto Matrix4Identity = Matrix4::identity();

template <class Matrix>
constexpr bool operator==(const Matrix& lhs, const Matrix& rhs) noexcept {
    using std::ranges::equal;
    using value_type = Matrix::value_type;
    return equal(lhs, rhs,
        [](auto a, auto b) { return math::almost_equal(a, b, math::BOOK_EPSILON); });
}

constexpr Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs) noexcept {    
    return Matrix4{
    lhs[0] * rhs[0] + lhs[1] * rhs[4] + lhs[2] * rhs[8] + lhs[3] * rhs[12], //0,0
    lhs[0] * rhs[1] + lhs[1] * rhs[5] + lhs[2] * rhs[9] + lhs[3] * rhs[13], //0,1
    lhs[0] * rhs[2] + lhs[1] * rhs[6] + lhs[2] * rhs[10] + lhs[3] * rhs[14], //0,2 
    lhs[0] * rhs[3] + lhs[1] * rhs[7] + lhs[2] * rhs[11] + lhs[3] * rhs[15], //0,3
    lhs[4] * rhs[0] + lhs[5] * rhs[4] + lhs[6] * rhs[8] + lhs[7] * rhs[12],
    lhs[4] * rhs[1] + lhs[5] * rhs[5] + lhs[6] * rhs[9] + lhs[7] * rhs[13],
    lhs[4] * rhs[2] + lhs[5] * rhs[6] + lhs[6] * rhs[10] + lhs[7] * rhs[14],
    lhs[4] * rhs[3] + lhs[5] * rhs[7] + lhs[6] * rhs[11] + lhs[7] * rhs[15],
    lhs[8] * rhs[0] + lhs[9] * rhs[4] + lhs[10] * rhs[8] + lhs[11] * rhs[12],
    lhs[8] * rhs[1] + lhs[9] * rhs[5] + lhs[10] * rhs[9] + lhs[11] * rhs[13],
    lhs[8] * rhs[2] + lhs[9] * rhs[6] + lhs[10] * rhs[10] + lhs[11] * rhs[14],
    lhs[8] * rhs[3] + lhs[9] * rhs[7] + lhs[10] * rhs[11] + lhs[11] * rhs[15],
    lhs[12] * rhs[0] + lhs[13] * rhs[4] + lhs[14] * rhs[8] + lhs[15] * rhs[12], //3,0
    lhs[12] * rhs[1] + lhs[13] * rhs[5] + lhs[14] * rhs[9] + lhs[15] * rhs[13], //3,1
    lhs[12] * rhs[2] + lhs[13] * rhs[6] + lhs[14] * rhs[10] + lhs[15] * rhs[14], //3,2
    lhs[12] * rhs[3] + lhs[13] * rhs[7] + lhs[14] * rhs[11] + lhs[15] * rhs[15]};//3,3
}

constexpr Matrix3 operator*(const Matrix3& lhs, const Matrix3& rhs) noexcept {    
    return Matrix3{
    lhs[0] * rhs[0] + lhs[1] * rhs[4] + lhs[2] * rhs[8], //(0, 0)
    lhs[0] * rhs[1] + lhs[1] * rhs[5] + lhs[2] * rhs[9], //(0, 1)
    lhs[0] * rhs[2] + lhs[1] * rhs[6] + lhs[2] * rhs[10], //(0, 2)
    lhs[4] * rhs[0] + lhs[5] * rhs[4] + lhs[6] * rhs[8],
    lhs[4] * rhs[1] + lhs[5] * rhs[5] + lhs[6] * rhs[9],
    lhs[4] * rhs[2] + lhs[5] * rhs[6] + lhs[6] * rhs[10],
    lhs[8] * rhs[0] + lhs[9] * rhs[4] + lhs[10] * rhs[8], //(2, 0)
    lhs[8] * rhs[1] + lhs[9] * rhs[5] + lhs[10] * rhs[9], //(2, 1)
    lhs[8] * rhs[2] + lhs[9] * rhs[6] + lhs[10] * rhs[10]};//(2, 2)    
}

constexpr Matrix2 operator*(const Matrix2& lhs, const Matrix2& rhs) noexcept {    
    return Matrix2{
    lhs[0] * rhs[0] + lhs[1] * rhs[4], //(0, 0)
    lhs[0] * rhs[1] + lhs[1] * rhs[5], //(0, 1)
    lhs[4] * rhs[0] + lhs[5] * rhs[4], //(1, 0)
    lhs[4] * rhs[1] + lhs[5] * rhs[5]};//(1, 1)    
}

//multiplying arbitrarily sized (square) Matrices.
template <class Matrix>
constexpr auto operator*(const Matrix& lhs, const Matrix& rhs) noexcept {
    using size_type = typename Matrix::size_type;
    Matrix result;
    for (size_type row = 0; row < Matrix::ROWS; ++row) {
        for (size_type col = 0; col < Matrix::COLUMNS; ++col) {
            for (size_type i = 0; i < Matrix::COLUMNS; ++i) {
                result(row, col) += lhs(row, i) * rhs(i, col);
            }
        }
    }
    return result;
}

Tuple operator*(const Matrix4& lhs, const Tuple rhs) noexcept {
    const auto x = rhs.x * lhs[0] + rhs.y   * lhs[1] + rhs.z * lhs[2]  + rhs.w * lhs[3];
    const auto y = rhs.x * lhs[4] + rhs.y   * lhs[5] + rhs.z * lhs[6]  + rhs.w * lhs[7];
    const auto z = rhs.x * lhs[8] + rhs.y   * lhs[9] + rhs.z * lhs[10] + rhs.w * lhs[11];
    const auto w = rhs.x * lhs[12] + rhs.y  * lhs[13]+ rhs.z * lhs[14] + rhs.w * lhs[15];
    return Tuple{ x, y, z, w };
}

template <class Matrix>
Matrix transpose(const Matrix& a) noexcept {
    using size_type = typename Matrix::size_type;
    Matrix result;
    for (size_type row = 0; row < Matrix::ROWS; ++row) {
        for (size_type col = 0; col < Matrix::COLUMNS; ++col) {
            result(col, row) = a(row, col);
        }
    }
    return result;
}

constexpr Real determinant(const Matrix1& m) noexcept {    
    return m[0]; //necessary for the template deduction
}

constexpr Real determinant(const Matrix2& m) noexcept {
    return m[0] * m[3] - m[1] * m[2];
}

constexpr Real determinant(const Matrix3& m) noexcept {
    return m[0] * (m[4] * m[8] - m[5] * m[7]) - 
           m[1] * (m[3] * m[8] - m[5] * m[6]) + 
           m[2] * (m[3] * m[7] - m[4] * m[6]);
}

constexpr Real determinant(const Matrix4& m) noexcept {
    return 
        m[0] * 
            (m[5] * m[10] * m[15] + m[6] * m[11] * m[13] + 
             m[7] * m[9] * m[14] - m[7] * m[10] * m[13] - 
             m[5] * m[11] * m[14] - m[6] * m[9] * m[15]) - 
        m[1] * 
            (m[4] * m[10] * m[15] + m[6] * m[11] * m[12] + 
            m[7] * m[8] * m[14] - m[7] * m[10] * m[12] - 
            m[4] * m[11] * m[14] - m[6] * m[8] * m[15]) +
        m[2] * 
            (m[4] * m[9] * m[15] + m[5] * m[11] * m[12] + 
            m[7] * m[8] * m[13] - m[7] * m[9] * m[12] - 
            m[4] * m[11] * m[13] - m[5] * m[8] * m[15]) - 
        m[3] * 
            (m[4] * m[9] * m[14] + m[5] * m[10] * m[12] + 
             m[6] * m[8] * m[13] - m[6] * m[9] * m[12] - 
             m[4] * m[10] * m[13] - m[5] * m[8] * m[14]);
}

//calculate determinant on arbitrary Matrix sizes.
template <class Matrix>
constexpr Real determinant2(const Matrix& m) noexcept {        
    using size_type = typename Matrix::size_type;
    Real out = 0;
    for (size_type col = 0; col < Matrix::COLUMNS; col++) {
        out += m[col] * cofactor(m, 0, col);
    }
    return out;
}

template <class Matrix>
constexpr auto submatrix(const Matrix& in, uint8_t remove_row, uint8_t remove_column) noexcept {
    assert(remove_row < Matrix::ROWS && remove_column < Matrix::COLUMNS && "submatrix() arguments are out of range. row and column must be inside the input matrix.");
    using size_type = typename Matrix::size_type;
    using SubMatrix = Matrix::submatrix_type;    
    SubMatrix out;
    size_type ri = 0;
    for (size_type i = 0; i < Matrix::ROWS; i++) {
        if (i == remove_row) { continue; }
        for (size_type j = 0; j < Matrix::COLUMNS; j++) {
            if (j == remove_column) { continue; }
            out[ri++] = in[i * Matrix::COLUMNS + j];
        }
    }
    return out;
}

//Returns the determinant of a submatrix
template <class Matrix>
constexpr Real minor(const Matrix& in, uint8_t remove_row, uint8_t remove_column) noexcept {
    assert(remove_row < Matrix::ROWS && remove_column < Matrix::COLUMNS && "minor() arguments are out of range. row and column must be inside the input matrix.");
    const auto sub = submatrix(in, remove_row, remove_column);
    return determinant(sub);
}

template <class Matrix>
constexpr Real cofactor(const Matrix& in, uint8_t remove_row, uint8_t remove_column) noexcept {
    assert(remove_row < Matrix::ROWS && remove_column < Matrix::COLUMNS && "cofactor() arguments are out of range. row and column must be inside the input matrix.");
    const auto min = minor(in, remove_row, remove_column);
    return math::is_odd(remove_row + remove_column) ? -min : min;
}

template <class Matrix>
constexpr bool is_invertible(const Matrix& m) noexcept {        
    return determinant(m) != 0;
}

template <class Matrix>
constexpr Matrix inverse(const Matrix& in) noexcept {        
    const auto det = determinant(in);
    assert(det != 0 && "Matrix inverse called with non-invertible Matrix");    
    const auto invDet = 1.0f / det;
    using size_type = typename Matrix::size_type;          
    Matrix result; 
    for (size_type row = 0; row < Matrix::ROWS; ++row) {
        for (size_type col = 0; col < Matrix::COLUMNS; ++col) {            
            result(col, row) = cofactor(in, row, col) * invDet; 
            //note: "col,row" instead of "row,col", because we want to transpose the result.
        }
    }    
    return result;
}

constexpr Matrix4 translation(Real x, Real y, Real z) noexcept {            
    return {
        1.0f, 0.0f, 0.0f,   x,
        0.0f, 1.0f, 0.0f,   y,
        0.0f, 0.0f, 1.0f,   z,
        0.0f, 0.0f, 0.0f,   1.0f
    };
}
constexpr Matrix4 translation(Point p) noexcept {                
    return translation(p.x, p.y, p.z);
}

constexpr Matrix4 scaling(Real x, Real y, Real z) noexcept {
    return {
        x,    0.0f, 0.0f,   0.0f,
        0.0f, y,    0.0f,   0.0f,
        0.0f, 0.0f, z,      0.0f,
        0.0f, 0.0f, 0.0f,   1.0f
    };
}
constexpr Matrix4 scaling(Point p) noexcept {                
    return scaling(p.x, p.y, p.z);
}

/*constexpr*/ Matrix4 rotation_x(Real radians) noexcept {
    const auto cos = std::cos(radians); //not available as constexpr yet...
    const auto sin = std::sin(radians);
    return {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos,  -sin, 0.0f,
        0.0f, sin,  cos,  0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}
/*constexpr*/ Matrix4 rotation_y(Real radians) noexcept {
    const auto cos = std::cos(radians); //not available as constexpr yet...
    const auto sin = std::sin(radians);
    return {
        cos,  0.0f, sin,  0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin, 0.0f, cos,  0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}
/*constexpr*/ Matrix4 rotation_z(Real radians) noexcept {
    const auto cos = std::cos(radians); //not available as constexpr yet...
    const auto sin = std::sin(radians);
    return {
        cos,  -sin, 0.0f, 0.0f,
        sin,  cos,  0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

/*constexpr*/ Matrix4 rotation(Real radians_x, Real radians_y, Real radians_z) noexcept {    
    //return rotation_x(radians_x) * rotation_y(radians_y) * rotation_z(radians_z);   
    const auto cos_x = std::cos(radians_x);
    const auto sin_x = -std::sin(radians_x);
    const auto cos_y = std::cos(radians_y);
    const auto sin_y = -std::sin(radians_y);
    const auto cos_z = std::cos(radians_z);
    const auto sin_z = -std::sin(radians_z);    
    return {
        cos_y * cos_z,                          cos_y * sin_z,                          -sin_y,         0.0f,
        sin_x * sin_y * cos_z - cos_x * sin_z,  sin_x * sin_y * sin_z + cos_x * cos_z,  sin_x * cos_y,  0.0f,
        cos_x * sin_y * cos_z + sin_x * sin_z,  cos_x * sin_y * sin_z - sin_x * cos_z,  cos_x * cos_y,  0.0f,
        0.0f,                                   0.0f,                                   0.0f,           1.0f
    };
}


constexpr Matrix4 shearing(Real Xy, Real Xx, Real Yx, Real Yz, Real Zx, Real Zy) noexcept {    
    return {
        1.0f,   Xy,     Xx,     0.0f,
        Yx,     1.0f,   Yz,     0.0f,
        Zx,     Zy,     1.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}