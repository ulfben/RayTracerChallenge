#pragma once
#include "pch.h"
#include "Math.h"

struct Point final {
    Real x{};
    Real y{};
    Real z{};
};
struct Vector final {
    Real x{};
    Real y{};
    Real z{};
};
struct UVCoords final {
    Real u{};
    Real v{};
};
constexpr bool operator==(const UVCoords& lhs, const UVCoords& rhs) noexcept {
    using math::float_cmp;
    return float_cmp(lhs.u, rhs.u) && float_cmp(lhs.v, rhs.v);
};


static constexpr Point ORIGO = Point{ 0, 0, 0 };

constexpr Vector normalize(const Vector& t) noexcept; //pre-declare

constexpr UVCoords uv(Real u, Real v) noexcept {
    return { u, v };
}

constexpr Vector vector(Real x, Real y, Real z) noexcept {
    return Vector{ x, y, z };
}
constexpr Vector vector(const Point& p) noexcept {
    return Vector{ p.x, p.y, p.z };
}
constexpr Vector vector(Real xyz) noexcept {
    return Vector{ xyz, xyz, xyz };
}
constexpr Vector normal_vector(Real x, Real y, Real z) noexcept {
    return normalize(Vector{ x, y, z });
}

constexpr Point point(Real x, Real y, Real z) noexcept {
    return Point{ x, y, z };
}
constexpr Point point(const Vector& v) noexcept {
    return Point{ v.x, v.y, v.z };
}
constexpr Point point(Real xyz) noexcept {
    return Point{ xyz, xyz, xyz };
}

//Vector interface
constexpr Vector operator*(const Vector& lhs, Real scalar) noexcept {
    return Vector{ lhs.x * scalar, lhs.y * scalar, lhs.z * scalar };
}
constexpr void operator*=(Vector& lhs, Real scalar) noexcept {
    lhs.x *= scalar;
    lhs.y *= scalar;
    lhs.z *= scalar;
}
constexpr Vector operator/(const Vector& lhs, Real scalar) noexcept {
    return Vector{ lhs.x / scalar, lhs.y / scalar, lhs.z / scalar };
}
constexpr Vector operator+(const Vector& lhs, const Vector& rhs) noexcept {
    return Vector{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}
constexpr void operator+=(Vector& lhs, const Vector& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
};
constexpr Vector operator-(const Vector& lhs, const Vector& rhs)  noexcept {
    return Vector{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}
constexpr Vector operator-(const Vector& self)  noexcept { return Vector{ -self.x, -self.y, -self.z }; }
constexpr bool operator==(const Vector& lhs, const Vector& rhs) noexcept {
    using math::float_cmp;
    return float_cmp(lhs.x, rhs.x) && float_cmp(lhs.y, rhs.y) && float_cmp(lhs.z, rhs.z);
};

constexpr Real magnitudeSq(const Vector& t) noexcept {
    return ((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Real magnitude(const Vector& t) noexcept {
    //std::hypot(t.x, t.y, t.z); 
    return math::sqrt((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Vector normalize(const Vector& t) noexcept {
    const auto length = magnitude(t);    
    if (math::is_zero(length)) {
        return t;
    }
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
constexpr bool is_normalized(const Vector& v, Real tolerance = math::BRAZZY_EPSILON) noexcept {
    const auto mag = magnitude(v);
    return math::abs(mag - 1.0f) <= tolerance;
}

constexpr bool is_zero(const Vector& v, Real tolerance = math::BRAZZY_EPSILON) noexcept {
    return magnitude(v) <= tolerance;
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

constexpr Vector hadamard_product(const Vector& lhs, const Vector& rhs) noexcept {
    return {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z,
    };
}

/*constexpr*/ Vector rodriguesRotation(const Vector& vector, const Vector& axis, Real angle) {
    const Vector unitAxis = normalize(axis);
    Real cosAngle = std::cos(angle);
    Real sinAngle = std::sin(angle);    
    return vector * cosAngle + unitAxis * ((1.0f - cosAngle) * dot(vector, unitAxis)) + cross(unitAxis, vector) * sinAngle;
}


//Point interface
constexpr Point operator/(const Point& lhs, Real scalar) noexcept {
    return Point{ lhs.x / scalar, lhs.y / scalar, lhs.z / scalar };
}
constexpr Point operator-(const Point& self) noexcept { return Point{ -self.x, -self.y, -self.z }; }
constexpr bool operator==(const Point& lhs, const Point& rhs) noexcept {
    using math::float_cmp;
    return float_cmp(lhs.x, rhs.x) && float_cmp(lhs.y, rhs.y) && float_cmp(lhs.z, rhs.z);
};
constexpr Point operator+(const Point& lhs, const Vector& rhs)  noexcept {
    return Point{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}
constexpr void operator+=(Point& lhs, const Vector& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
};
constexpr Vector operator-(const Point& lhs, const Point& rhs)  noexcept {
    return Vector{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}
constexpr Point operator-(const Point& lhs, const Vector& rhs) noexcept {
    return Point{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

//printing support:
template <>
struct std::formatter<Vector> : std::formatter<string_view> {
    auto format(const Vector& obj, std::format_context& ctx) const {
        std::string temp;
        std::format_to(std::back_inserter(temp), "({}, {}, {})"sv,
            obj.x, obj.y, obj.z);
        return std::formatter<string_view>::format(temp, ctx);
    }
};

#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Vector& t) {
    os << std::format("{}, {}, {}"sv, t.x, t.y, t.z);
    return os;
}
#pragma warning(pop)

