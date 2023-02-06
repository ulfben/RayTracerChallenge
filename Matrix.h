#pragma once
#include "pch.h"
#include "Tuple.h"

template <uint8_t ROWS, uint8_t COLUMNS> struct Matrix {
    static_assert((uint16_t(ROWS)* COLUMNS) < std::numeric_limits<uint8_t>::max(), "Matrix is limited to 255 elements.");
    using value_type = Real;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = uint8_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    static constexpr size_type HEIGHT = ROWS;
    static constexpr size_type WIDTH = COLUMNS;

    value_type _data[COLUMNS * ROWS]{};

    constexpr reference operator()(size_type row, size_type col) noexcept {
        assert(row < rows() && col < columns() && "Matrix access is out of bounds");
        return _data[columns() * row + col];
    }
    constexpr value_type operator()(size_type row, size_type col) const noexcept {
        assert(row < rows() && col < columns() && "Matrix access is out of bounds");
        return _data[columns() * row + col];
    }
    constexpr value_type operator[](size_type i) const noexcept {
        assert(i < size());
        return _data[i];
    }
    constexpr reference operator[](size_type i) noexcept {
        assert(i < size());
        return _data[i];
    }

    constexpr size_type index_to_column(size_type index) const noexcept {
        assert(index < size());
        return index % columns();
    }
    constexpr size_type index_to_row(size_type index) const noexcept {
        assert(index < size());
        return index / columns();
    }
        
    constexpr auto submatrix(size_type remove_row, size_type remove_column) const noexcept {
        assert(remove_row < rows() && remove_column < columns() && "submatrix: row and column to remove must be inside the original matrix.");
        Matrix<ROWS - 1, COLUMNS - 1> r;          
        uint8_t ri = 0;
        for (size_type i = 0; i < size(); i++) {
            //if(index_to_row(i) == remove_row) { i += columns(); } //skip the entire row
            if(index_to_row(i) != remove_row && index_to_column(i) != remove_column) {
                r[ri++] = _data[i];
            }
        }     
        return r;
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

    static constexpr auto identity() noexcept {
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
using Matrix1 = Matrix<1, 1>; //to stop template deductions from breaking
static constexpr auto Matrix4Identity = Matrix4::identity();

template <class Matrix>
constexpr bool operator==(const Matrix& lhs, const Matrix& rhs) noexcept {
    using std::ranges::equal;
    return equal(lhs, rhs,
        [](auto a, auto b) { return math::almost_equal(a, b); });
}

constexpr Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs) noexcept {    
    Matrix4 result;
    result(0, 0) = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0) + lhs(0, 3) * rhs(3, 0);
    result(0, 1) = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1) + lhs(0, 3) * rhs(3, 1);
    result(0, 2) = lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2) + lhs(0, 3) * rhs(3, 2);
    result(0, 3) = lhs(0, 0) * rhs(0, 3) + lhs(0, 1) * rhs(1, 3) + lhs(0, 2) * rhs(2, 3) + lhs(0, 3) * rhs(3, 3);
    result(1, 0) = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0) + lhs(1, 3) * rhs(3, 0);
    result(1, 1) = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1) + lhs(1, 3) * rhs(3, 1);
    result(1, 2) = lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2) + lhs(1, 3) * rhs(3, 2);
    result(1, 3) = lhs(1, 0) * rhs(0, 3) + lhs(1, 1) * rhs(1, 3) + lhs(1, 2) * rhs(2, 3) + lhs(1, 3) * rhs(3, 3);
    result(2, 0) = lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0) + lhs(2, 3) * rhs(3, 0);
    result(2, 1) = lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1) + lhs(2, 3) * rhs(3, 1);
    result(2, 2) = lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2) + lhs(2, 3) * rhs(3, 2);
    result(2, 3) = lhs(2, 0) * rhs(0, 3) + lhs(2, 1) * rhs(1, 3) + lhs(2, 2) * rhs(2, 3) + lhs(2, 3) * rhs(3, 3);
    result(3, 0) = lhs(3, 0) * rhs(0, 0) + lhs(3, 1) * rhs(1, 0) + lhs(3, 2) * rhs(2, 0) + lhs(3, 3) * rhs(3, 0);
    result(3, 1) = lhs(3, 0) * rhs(0, 1) + lhs(3, 1) * rhs(1, 1) + lhs(3, 2) * rhs(2, 1) + lhs(3, 3) * rhs(3, 1);
    result(3, 2) = lhs(3, 0) * rhs(0, 2) + lhs(3, 1) * rhs(1, 2) + lhs(3, 2) * rhs(2, 2) + lhs(3, 3) * rhs(3, 2);
    result(3, 3) = lhs(3, 0) * rhs(0, 3) + lhs(3, 1) * rhs(1, 3) + lhs(3, 2) * rhs(2, 3) + lhs(3, 3) * rhs(3, 3);
    return result;
}

constexpr Matrix3 operator*(const Matrix3& lhs, const Matrix3& rhs) noexcept {    
    Matrix3 result;
    result(0, 0) = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0);
    result(0, 1) = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1);
    result(0, 2) = lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2);
    result(1, 0) = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0);
    result(1, 1) = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1);
    result(1, 2) = lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2);
    result(2, 0) = lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0);
    result(2, 1) = lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1);
    result(2, 2) = lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2);
    return result;
}

constexpr Matrix2 operator*(const Matrix2& lhs, const Matrix2& rhs) noexcept {    
    Matrix2 result;    
    result(0, 0) = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0);
    result(0, 1) = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1);
    result(1, 0) = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0);
    result(1, 1) = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1);
    return result;
}

//multiplying arbitrarily sized (square) Matrices.
template <class Matrix>
constexpr auto operator*(const Matrix& lhs, const Matrix& rhs) noexcept {
    using size_type = Matrix::size_type;
    Matrix result;
    for (size_type row = 0; row < Matrix::HEIGHT; ++row) {
        for (size_type col = 0; col < Matrix::WIDTH; ++col) {
            for (size_type i = 0; i < Matrix::WIDTH; ++i) {
                result(row, col) += lhs(row, i) * rhs(i, col);
            }
        }
    }
    return result;
}

Tuple operator*(const Matrix4& lhs, const Tuple rhs) noexcept {
    const auto x = rhs.x * lhs(0, 0) + rhs.y * lhs(0, 1) + rhs.z * lhs(0, 2) + rhs.w * lhs(0, 3);
    const auto y = rhs.x * lhs(1, 0) + rhs.y * lhs(1, 1) + rhs.z * lhs(1, 2) + rhs.w * lhs(1, 3);
    const auto z = rhs.x * lhs(2, 0) + rhs.y * lhs(2, 1) + rhs.z * lhs(2, 2) + rhs.w * lhs(2, 3);
    const auto w = rhs.x * lhs(3, 0) + rhs.y * lhs(3, 1) + rhs.z * lhs(3, 2) + rhs.w * lhs(3, 3);
    return Tuple{ x, y, z, w };
}

template <uint8_t ROWS, uint8_t COLUMNS>
Matrix<ROWS, COLUMNS> transpose(const Matrix<ROWS, COLUMNS>& a) noexcept {
    Matrix<ROWS, COLUMNS> result;
    for (uint8_t row = 0; row < ROWS; ++row) {
        for (uint8_t col = 0; col < COLUMNS; ++col) {
            result(col, row) = a(row, col);
        }
    }
    return result;
}

constexpr Real determinant(const Matrix1& m) noexcept {    
    return m(0, 0); //necessary for the template deduction
}

constexpr Real determinant(const Matrix2& m) noexcept {
    return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
}

constexpr Real determinant(const Matrix3& m) noexcept {
    return m(0,0) * (m(1,1) * m(2,2) - m(1,2) * m(2,1)) - 
           m(0,1) * (m(1,0) * m(2,2) - m(1,2) * m(2,0)) + 
           m(0,2) * (m(1,0) * m(2,1) - m(1,1) * m(2,0));
}

constexpr Real determinant(const Matrix4& m) noexcept {
    return 
        m(0,0) * 
            (m(1,1) * m(2,2) * m(3,3) + m(1,2) * m(2,3) * m(3,1) + 
             m(1,3) * m(2,1) * m(3,2) - m(1,3) * m(2,2) * m(3,1) - 
             m(1,1) * m(2,3) * m(3,2) - m(1,2) * m(2,1) * m(3,3)) - 
        m(0,1) * 
            (m(1,0) * m(2,2) * m(3,3) + m(1,2) * m(2,3) * m(3,0) + 
            m(1,3) * m(2,0) * m(3,2) - m(1,3) * m(2,2) * m(3,0) - 
            m(1,0) * m(2,3) * m(3,2) - m(1,2) * m(2,0) * m(3,3)) +
        m(0,2) * 
            (m(1,0) * m(2,1) * m(3,3) + m(1,1) * m(2,3) * m(3,0) + 
            m(1,3) * m(2,0) * m(3,1) - m(1,3) * m(2,1) * m(3,0) - 
            m(1,0) * m(2,3) * m(3,1) - m(1,1) * m(2,0) * m(3,3)) - 
        m(0,3) * 
            (m(1,0) * m(2,1) * m(3,2) + m(1,1) * m(2,2) * m(3,0) + 
             m(1,2) * m(2,0) * m(3,1) - m(1,2) * m(2,1) * m(3,0) - 
             m(1,0) * m(2,2) * m(3,1) - m(1,1) * m(2,0) * m(3,2));
}

//calculate determinant on arbitrary Matrix sizes.
template <uint8_t ROWS, uint8_t COLUMNS>
constexpr Real determinant2(const Matrix<ROWS, COLUMNS>& m) noexcept {        
    using size_type = Matrix<ROWS, COLUMNS>::size_type;
    Real out = 0;
    for (size_type col = 0; col < COLUMNS; col++) {
        out += m(0, col) * cofactor(m, 0, col);
    }
    return out;
}

template <uint8_t ROWS, uint8_t COLUMNS>
constexpr auto submatrix(const Matrix<ROWS, COLUMNS>& in, uint8_t remove_row, uint8_t remove_column) noexcept {
    assert(remove_row < ROWS&& remove_column < COLUMNS && "invalid submatrix specification. row and column must be inside the input matrix.");
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

//Returns the determinant of a submatrix
template <uint8_t ROWS, uint8_t COLUMNS>
constexpr Real minor(const Matrix<ROWS, COLUMNS>& in, uint8_t remove_row, uint8_t remove_column) noexcept {
    assert(remove_row < ROWS&& remove_column < COLUMNS && "invalid submatrix specification. row and column must be inside the input matrix.");
    const auto sub = submatrix(in, remove_row, remove_column);
    return determinant(sub);
}

template <uint8_t ROWS, uint8_t COLUMNS>
constexpr Real cofactor(const Matrix<ROWS, COLUMNS>& in, uint8_t remove_row, uint8_t remove_column) noexcept {
    assert(remove_row < ROWS&& remove_column < COLUMNS && "invalid submatrix specification. row and column must be inside the input matrix.");
    const auto min = minor(in, remove_row, remove_column);
    return math::is_odd(remove_row + remove_column) ? -min : min;
}