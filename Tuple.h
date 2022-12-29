#pragma once
#include "pch.h"
struct Tuple;
using Real = float;
using Vector = Tuple;
using Point = Tuple;
using Color = Tuple;
constexpr Color hadamard_product(const Color &a, const Color &b) noexcept;

struct Tuple {
  union {
    Real data[4];
    struct {
      Real x, y, z, w;
    };
    struct {
      Real r, g, b, a;
    };
  };

  constexpr Tuple operator*(const Tuple &color) const noexcept {
    return hadamard_product(*this, color);
  }

  constexpr Tuple operator*(Real scalar) const noexcept {
    return Tuple{x * scalar, y * scalar, z * scalar, w * scalar};
  }
  constexpr Tuple operator/(Real scalar) const noexcept {
    return Tuple{x / scalar, y / scalar, z / scalar, w / scalar};
  }
  constexpr Tuple operator+(Tuple rhs) const noexcept {
    return Tuple{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
  }
  constexpr Tuple operator-(Tuple rhs) const noexcept {
    return Tuple{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
  }
  constexpr Tuple operator-() const noexcept { return Tuple{-x, -y, -z, w}; }
  constexpr bool operator==(const Tuple &rhs) const noexcept {
    return rhs.x == x && rhs.y == y && rhs.z == z && rhs.w == w;
  };
};



constexpr std::string to_string(Color c) { return ""; }

constexpr Tuple vector(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 0.0f};
}
constexpr Tuple point(Real x, Real y, Real z) noexcept {
  return Tuple{x, y, z, 1.0f};
}
constexpr Tuple color(Real r, Real g, Real b, Real a = 1.0f) noexcept {
  return Tuple{r, g, b, a};
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