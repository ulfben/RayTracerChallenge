#pragma once
#include "pch.h"
#include "Math.h"

struct Tuple;
struct Color;
using Real = float;
using Vector = Tuple;
using Point = Tuple;
constexpr Color hadamard_product(const Color &a, const Color &b) noexcept;

struct Color final {    
    Real r{};
    Real g{};
    Real b{};
  constexpr Color operator*(const Color &color) const noexcept {
    return hadamard_product(*this, color);
  }
  constexpr Color operator*(Real scalar) const noexcept {
    return Color{r * scalar, g * scalar, b * scalar};
  }
  constexpr void operator*=(Real scalar) noexcept {
    r *= scalar;
    g *= scalar;
    b *= scalar;    
  }
  constexpr Color operator/(Real scalar) const noexcept {
    return Color{r / scalar, g / scalar, b / scalar};
  }
  constexpr Color operator+(Color rhs) const noexcept {
    return Color{r + rhs.r, g + rhs.g, b + rhs.b};
  }
  constexpr Color operator-(Color rhs) const noexcept {
    return Color{r - rhs.r, g - rhs.g, b - rhs.b};
  } 
  constexpr bool operator==(const Color &rhs) const noexcept {
    using math::float_cmp;
    return float_cmp(r, rhs.r) && float_cmp(g, rhs.g) &&
           float_cmp(b, rhs.b);
  };
};

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
    return Tuple{x * scalar, y * scalar, z * scalar, w * scalar};
  }
  constexpr void operator*=(Real scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
  }
  constexpr Tuple operator/(Real scalar) const noexcept {
    return Tuple{x / scalar, y / scalar, z / scalar, w / scalar};
  }
  constexpr Tuple operator+(const Tuple& rhs) const noexcept {
    return Tuple{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
  }
  constexpr void operator+=(const Tuple& rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
  };
  constexpr Tuple operator-(const Tuple& rhs) const noexcept {
    return Tuple{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
  }
  constexpr Tuple operator-() const noexcept { return Tuple{-x, -y, -z, w}; }
  constexpr bool operator==(const Tuple &rhs) const noexcept {
    using math::float_cmp;
    return float_cmp(x, rhs.x) && float_cmp(y, rhs.y) &&
           float_cmp(z, rhs.z) && float_cmp(w, rhs.w);
  };
};

std::ostream& operator<<(std::ostream& os, const Tuple&t){
    os << std::format("{}, {}, {}, {}", t.x, t.y, t.z, t.w);
    return os;
}

std::ostream& operator<<(std::ostream& os, const Color&t){
    os << std::format("{}, {}, {}", t.r, t.g, t.b);
    return os;
}

constexpr Vector vector(Real x, Real y, Real z) noexcept {
  return Vector{x, y, z, 0.0f};
}
constexpr Point point(Real x, Real y, Real z) noexcept {
  return Point{x, y, z, 1.0f};
}
constexpr Color color(Real r, Real g, Real b) noexcept {
  return Color{r, g, b};
}

constexpr Color to_byte_values(Color col) noexcept {
  const float MAX = 255;
  return color(std::clamp(std::round(col.r * MAX), 0.0f, MAX),
               std::clamp(std::round(col.g * MAX), 0.0f, MAX),
               std::clamp(std::round(col.b * MAX), 0.0f, MAX));
}

std::string to_rgb_bytes(Color col) {
  const auto out = to_byte_values(col);
  return std::format("{} {} {}", out.r, out.g, out.b);
}

constexpr bool is_vector(Tuple t) noexcept { return t.w == 0; }
constexpr bool is_point(Tuple t) noexcept { return t.w == 1; }

constexpr Real magnitudeSq(const Vector &t) noexcept {
  return ((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Real magnitude(const Vector &t) noexcept {
  return math::sqrt((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Vector normalize(const Vector &t) noexcept {
  const auto length = magnitude(t);
  assert(length != 0.0f);
  return t / length;
}
constexpr Real dot(const Vector &a, const Vector &b) noexcept {
  /*
 the smaller the dot product, the larger th angle between the vector
 if the two vectors are unit vectors, the dot product is the cosine of the angle
 between them
 */
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
constexpr Vector cross(const Vector &a, const Vector &b) noexcept {
  return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

constexpr Color hadamard_product(const Color &a, const Color &b) noexcept {
  return Color{a.r * b.r, a.g * b.g, a.b * b.b};
}

constexpr Vector reflect(const Vector &v, const Vector& normal) noexcept {  
  return v - normal * 2 * dot(v, normal);
}