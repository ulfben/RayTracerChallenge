#pragma once
#include "pch.h"
#include "Math.h"

struct Tuple final {    
    Real x{};
    Real y{};            
    Real z{};
    Real w{};

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

template <>
struct std::formatter<Tuple> : std::formatter<string_view> {
    auto format(const Tuple& obj, std::format_context& ctx) const {
        std::string temp;
        std::format_to(std::back_inserter(temp), "({}, {}, {}, {})"sv, 
                       obj.x, obj.y, obj.z, obj.w);
        return std::formatter<string_view>::format(temp, ctx);
    }
};


#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Tuple& t) {
    os << std::format("{}, {}, {}, {}"sv, t.x, t.y, t.z, t.w);
    return os;
}
#pragma warning(pop)

constexpr Vector vector(Real x, Real y, Real z) noexcept {
    return Vector{ x, y, z, 0.0f };
}
constexpr Point point(Real x, Real y, Real z) noexcept {
    return Point{ x, y, z, 1.0f };
}

static constexpr Point ORIGO = point(0,0,0);

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
    const auto inv_length = 1.0f / length;
    return t * inv_length;
}
constexpr Vector normalize(const Vector& t, const Vector& fallback) noexcept {
    const auto length = magnitude(t);
    if (math::is_zero(length)) {
        return fallback;
    }
    const auto inv_length = 1.0f / length;
    return t * inv_length;
}
constexpr Real dot(const Vector& a, const Vector& b) noexcept {
    /* the smaller the dot product, the larger the angle between the vector. if the two
    vectors are unit vectors, the dot product is the cosine of the angle between them */
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
constexpr Vector cross(const Vector& a, const Vector& b) noexcept {
    return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

constexpr Vector reflect(const Vector& v, const Vector& normal) noexcept {
    return v - normal * 2 * dot(v, normal);
}