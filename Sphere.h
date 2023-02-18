#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Lights.h"

struct Sphere final {
    Matrix4 transform{ Matrix4Identity }; 
    Material surface{ material() }; 
    Point position{0,0,0};
    Real radius{0};
    constexpr Sphere() noexcept = default;
    constexpr Sphere(Point p, Real rad) noexcept : position(p), radius(rad)
    {}
    constexpr Sphere(Point p, Real rad, Material m) noexcept : surface(m), position(p), radius(rad)
    {}
    constexpr bool operator==(const Sphere& that) const noexcept {
        return position == that.position && math::float_cmp(radius, that.radius)
            && surface == that.surface && transform == that.transform;
    }
};

constexpr Sphere sphere(Point p, Real radius) noexcept {
    return Sphere{ p, radius };
}
constexpr Sphere sphere(Material m) noexcept {
    return Sphere{ point(0,0,0), 1.0f, std::move(m) };    
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

constexpr Vector normal_at(const Sphere* s, const Point& p) noexcept {
    assert(s && "normal at called with nullptr. Make sure your hit is non-empty before using it!");
    return normal_at(*s, p);
}