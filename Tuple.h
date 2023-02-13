#pragma once
#include "pch.h"
#include "Math.h"

struct Tuple;
using Real = float;
using Vector = Tuple;
using Point = Tuple;
using Color = Tuple;
constexpr Color hadamard_product(const Color &a, const Color &b) noexcept;

#pragma warning(push)
#pragma warning( disable : 4201 )
//warning C4201: nonstandard extension used: nameless struct/union
struct Tuple final {
    static const auto SIZE = 4;
  union {
      Real data[SIZE]{};
    struct {
      Real x, y, z, w;
    };
    struct{
      Real r, g, b, a;
    };
  };
#pragma warning(pop)

  constexpr Tuple operator*(const Tuple &color) const noexcept {
    return hadamard_product(*this, color);
  }

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
  constexpr Tuple operator+(Tuple rhs) const noexcept {
    return Tuple{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
  }
  constexpr void operator+=(Tuple rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
  };
  constexpr Tuple operator-(Tuple rhs) const noexcept {
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
    os << t.x << ", " << t.y << ", " << t.z << ", " << t.w;
    return os;
}

constexpr Tuple vector(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 0.0f};
}
constexpr Tuple point(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 1.0f};
}
constexpr Tuple color(Real r, Real g, Real b, Real a = 1.0f) noexcept {
  return Tuple{r, g, b, a};
}

constexpr Color to_byte_values(Color c) noexcept {
  // TODO: implement expects(), requires() etc.
  // assert(c.r >= 0 && c.r < 1.0f && "to_rgb(): red channel is out of range");
  // assert(c.g >= 0 && c.g < 1.0f && "to_rgb(): green channel is out of
  // range"); assert(c.b >= 0 && c.b < 1.0f && "to_rgb(): blue channel is out of
  // range");
  const float MAX = PPM_MAX;
  return color(std::clamp(std::round(c.r * PPM_MAX), 0.0f, MAX),
               std::clamp(std::round(c.g * PPM_MAX), 0.0f, MAX),
               std::clamp(std::round(c.b * PPM_MAX), 0.0f, MAX));
}

std::string to_rgb_bytes(Color c) {
  const auto out = to_byte_values(c);
  return std::format("{} {} {}", out.r, out.g, out.b);
}

constexpr bool is_vector(Tuple t) noexcept { return t.w == 0; }
constexpr bool is_point(Tuple t) noexcept { return t.w == 1; }

constexpr Real magnitudeSq(const Tuple &t) noexcept {
  return ((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Real magnitude(const Tuple &t) noexcept {
  return math::sqrt((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}
constexpr Tuple normalize(const Tuple &t) noexcept {
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
constexpr Tuple cross(const Tuple &a, const Tuple &b) noexcept {
  return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

constexpr Tuple hadamard_product(const Color &a, const Color &b) noexcept {
  return Color{a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a};
}