#pragma once
#include "pch.h"
#include "Math.h"

struct Color final {
    Real r{};
    Real g{};
    Real b{};
    constexpr Color operator*(const Color& that) const noexcept {
        return { r * that.r, g * that.g, b * that.b };
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

static constexpr Color BLACK = color(0, 0, 0);
static constexpr Color WHITE = color(1, 1, 1);
static constexpr Color RED = color(1, 0, 0);
static constexpr Color GREEN = color(0, 1, 0);
static constexpr Color BLUE = color(0, 0, 1);

#pragma warning(push)
#pragma warning( disable : 4201 )
//warning C4201: nonstandard extension used: nameless struct/union
struct Tuple final {
    static const auto SIZE = 4;
    union {
        Real data[4]{};
        struct {
            Real x, y, z, w;
        };
    };
#pragma warning(pop)

    constexpr Tuple operator*(Real scalar) const noexcept {
        return Tuple{ x * scalar, y * scalar, z * scalar, w * scalar };
    }
    constexpr void operator*=(Real scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
    }
    constexpr Tuple operator/(Real scalar) const noexcept {
        return Tuple{ x / scalar, y / scalar, z / scalar, w / scalar };
    }
    constexpr Tuple operator+(const Tuple& rhs) const noexcept {
        return Tuple{ x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
    }
    constexpr void operator+=(const Tuple& rhs) noexcept {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
    };
    constexpr Tuple operator-(const Tuple& rhs) const noexcept {
        return Tuple{ x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
    }
    constexpr Tuple operator-() const noexcept { return Tuple{ -x, -y, -z, w }; }
    constexpr bool operator==(const Tuple& rhs) const noexcept {
        using math::float_cmp;
        return float_cmp(x, rhs.x) && float_cmp(y, rhs.y) &&
            float_cmp(z, rhs.z) && float_cmp(w, rhs.w);
    };
};

#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Tuple& t) {
    os << std::format("{}, {}, {}, {}"sv, t.x, t.y, t.z, t.w);
    return os;
}

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

constexpr Vector vector(Real x, Real y, Real z) noexcept {
    return Vector{ x, y, z, 0.0f };
}
constexpr Point point(Real x, Real y, Real z) noexcept {
    return Point{ x, y, z, 1.0f };
}

constexpr ByteColor to_byte_color(Color col) noexcept {
    return ByteColor(col);
}

constexpr bool is_vector(Tuple t) noexcept { return t.w == 0; }
constexpr bool is_point(Tuple t) noexcept { return t.w == 1; }

constexpr Real magnitudeSq(const Vector& t) noexcept {
    return ((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Real magnitude(const Vector& t) noexcept {
    return math::sqrt((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Vector normalize(const Vector& t) noexcept {
    const auto length = magnitude(t);
    assert(length != 0.0f);
    return t / length;
}
constexpr Real dot(const Vector& a, const Vector& b) noexcept {
    /* the smaller the dot product, the larger the angle between the vector. if the two
    vectors are unit vectors, the dot product is the cosine of the angle between them */
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
constexpr Vector cross(const Vector& a, const Vector& b) noexcept {
    return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

constexpr Color hadamard_product(const Color& a, const Color& b) noexcept {
    return a * b;
}

constexpr Vector reflect(const Vector& v, const Vector& normal) noexcept {
    return v - normal * 2 * dot(v, normal);
}