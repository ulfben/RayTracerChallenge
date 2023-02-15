#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"

struct Sphere {
    Point position; 
    Real radius;
    Matrix4 transform{ Matrix4Identity };

    constexpr bool operator==(const Sphere& that) const noexcept {
        return position == that.position && math::float_cmp(radius, that.radius);
    }
};

constexpr Sphere sphere(Point p, Real radius) noexcept {
    return Sphere{ p, radius };
}
constexpr Sphere sphere() noexcept {
    return Sphere{ point(0,0,0), 1.0f };
}

constexpr Vector normal_at(const Sphere& s, const Point& p) noexcept { 
    const auto inv_transform = inverse(s.transform);
    const auto object_space_point = inv_transform * p; 
    const auto object_space_normal = object_space_point - s.position; /*s position is always 0*/
    auto world_space_normal = transpose(inv_transform) * object_space_normal;
    world_space_normal.w = 0; //hack to avoid having to work with the transform submatrix
    return normalize(world_space_normal);
}