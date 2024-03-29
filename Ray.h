#pragma once
#include "pch.h"
#include "Tuple.h"

struct Ray final {
    Point origin;
    Vector direction;
    constexpr bool operator==(const Ray& that) const noexcept = default;
    
    constexpr auto x() const noexcept { return origin.x; }
    constexpr auto y() const noexcept { return origin.y; }
    constexpr auto z() const noexcept { return origin.z; }
    constexpr auto dx() const noexcept { return direction.x; }
    constexpr auto dy() const noexcept { return direction.y; }
    constexpr auto dz() const noexcept { return direction.z; }
};
constexpr Ray ray(Point p, Vector dir) noexcept {    
    return Ray{ p, dir };
}

constexpr Ray operator*(const Matrix4& m, const Ray& r) noexcept {    
    return Ray{ m * r.origin, m * r.direction };
}

constexpr Ray operator*(const Ray& r, const Matrix4& m) noexcept {    
    return m * r;
}

constexpr Point position(const Ray& r, Real time) noexcept {    
    return r.origin + r.direction * time;
}

constexpr Ray transform(const Ray& r, const Matrix4& m) noexcept {    
    return m * r;
}