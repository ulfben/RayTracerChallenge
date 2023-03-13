#pragma once
#include "pch.h"
#include "Math.h"

struct Color final {
    Real r{};
    Real g{};
    Real b{};
    constexpr Color operator*(const Color& that) const noexcept {
        return { r * that.r, g * that.g, b * that.b }; //hadamard product
    }
    constexpr Color operator*(Real scalar) const noexcept {
        return Color{ r * scalar, g * scalar, b * scalar };
    }
    constexpr void operator*=(Real scalar) noexcept {
        r *= scalar;
        g *= scalar;
        b *= scalar;
    }
    constexpr Color operator/(Real scalar) const noexcept {
        return Color{ r / scalar, g / scalar, b / scalar };
    }
    constexpr Color operator+(Color rhs) const noexcept {
        return Color{ r + rhs.r, g + rhs.g, b + rhs.b };
    }
    constexpr Color operator-(Color rhs) const noexcept {
        return Color{ r - rhs.r, g - rhs.g, b - rhs.b };
    }
    constexpr bool operator==(const Color& rhs) const noexcept {
        using math::float_cmp;
        return float_cmp(r, rhs.r) && float_cmp(g, rhs.g) &&
            float_cmp(b, rhs.b);
    };
};
constexpr Color color(Real r, Real g, Real b) noexcept {
    return Color{ r, g, b };
}

//bytecolor is an optimization to help us speed up writing the image to files. 
//the idea is to bulk-convert the image-buffer into a smaller integer buffer, 
//which we can then split across threads to make strings out of.
struct ByteColor final {
    using value_type = uint8_t;
    static constexpr float MAX = PPM_MAX_BYTE_VALUE;    
    value_type r{};
    value_type g{};
    value_type b{};
    constexpr ByteColor() noexcept = default;
    constexpr explicit ByteColor(const Color& c) noexcept {
        r = map_to<value_type>(c.r);
        g = map_to<value_type>(c.g);
        b = map_to<value_type>(c.b);
        assert(r <= MAX && g <= MAX && b <= MAX);
        assert(r >= 0 && g >= 0 && b >= 0);
    };
};

constexpr ByteColor to_byte_color(Color col) noexcept {
    return ByteColor(col);
}

constexpr Color hadamard_product(const Color& a, const Color& b) noexcept {
    return a * b;
}

static constexpr Color BLACK = color(0, 0, 0);
static constexpr Color WHITE = color(1, 1, 1);
static constexpr Color RED = color(1, 0, 0);
static constexpr Color GREEN = color(0, 1, 0);
static constexpr Color BLUE = color(0, 0, 1);

#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Color& t) {
    os << std::format("{}, {}, {}"sv, t.r, t.g, t.b);
    return os;
}

std::string to_string(Color c) {
    return std::format("{} {} {}"sv, c.r, c.g, c.b);
}
std::string to_string(ByteColor c) {
    return std::format("{} {} {}"sv, c.r, c.g, c.b);
}
std::string to_string_with_trailing_space(ByteColor c) {
    return std::format("{} {} {} "sv, c.r, c.g, c.b);
}
#pragma warning(pop)