#pragma once
#include "pch.h"
#include "Tuple.h"

struct Ray final {
    Point origin;
    Vector direction;
    constexpr bool operator==(const Ray& that) const noexcept = default;
};
constexpr Ray ray(Point p, Vector dir) noexcept {
    assert(is_point(p));
    assert(is_vector(dir));
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