#pragma once
#include "pch.h"
#include "Math.h"

struct Point final {    
    Real x{};
    Real y{};            
    Real z{};   

    //constexpr Point operator*(Real scalar) const noexcept {
    //    return Point{ x * scalar, y * scalar, z * scalar, w * scalar };
    //}
    //constexpr void operator*=(Real scalar) noexcept {
    //    x *= scalar;
    //    y *= scalar;
    //    z *= scalar;
    //    w *= scalar;
    //}
    constexpr Point operator/(Real scalar) const noexcept {
        return Point{ x / scalar, y / scalar, z / scalar };
    }
    constexpr Point operator-() const noexcept { return Point{ -x, -y, -z }; }
    constexpr bool operator==(const Point& rhs) const noexcept {
        using math::float_cmp;
        return float_cmp(x, rhs.x) && float_cmp(y, rhs.y) && float_cmp(z, rhs.z);
    };
    
    /*constexpr Point operator+(const Vector& rhs) const noexcept {
        return Point{ x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
    }
    constexpr void operator+=(const Vector& rhs) noexcept {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
    };
    constexpr Vector operator-(const Point& rhs) const noexcept {
        return Vector{ x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
    }
    constexpr Point operator-(const Vector& rhs) const noexcept {
        return Point{ x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
    }*/
    
};

struct Vector final {    
    Real x{};
    Real y{};            
    Real z{};   

    constexpr Vector operator*(Real scalar) const noexcept {
        return Vector{ x * scalar, y * scalar, z * scalar  };
    }
    constexpr void operator*=(Real scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;        
    }
    constexpr Vector operator/(Real scalar) const noexcept {
        return Vector{ x / scalar, y / scalar, z / scalar };
    }
    constexpr Vector operator+(const Vector& rhs) const noexcept {
        return Vector{ x + rhs.x, y + rhs.y, z + rhs.z };
    }
    constexpr void operator+=(const Vector& rhs) noexcept {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;        
    };
    constexpr Vector operator-(const Vector& rhs) const noexcept {
        return Vector{ x - rhs.x, y - rhs.y, z - rhs.z };
    }
    constexpr Vector operator-() const noexcept { return Vector{ -x, -y, -z}; }
    constexpr bool operator==(const Vector& rhs) const noexcept {
        using math::float_cmp;
        return float_cmp(x, rhs.x) && float_cmp(y, rhs.y) && float_cmp(z, rhs.z);
    };
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


template <>
struct std::formatter<Vector> : std::formatter<string_view> {
    auto format(const Vector& obj, std::format_context& ctx) const {
        std::string temp;
        std::format_to(std::back_inserter(temp), "({}, {}, {})"sv, 
                       obj.x, obj.y, obj.z );
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

constexpr Vector vector(Real x, Real y, Real z) noexcept {
    return Vector{ x, y, z };
}
constexpr Vector vector(const Point& p) noexcept {
    return Vector{ p.x, p.y, p.z  };
}
constexpr Point point(Real x, Real y, Real z) noexcept {
    return Point{ x, y, z  }; //, 1.0f
}
constexpr Point point(const Vector& v) noexcept {
    return Point{ v.x, v.y, v.z };
}

static constexpr Point ORIGO = point(0,0,0);

constexpr bool is_vector(const Vector& t) noexcept { return true; }
constexpr bool is_point(const Point& p) noexcept { return true; }

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