#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"
/*A unit AABB, always positioned at 0, 0, 0 and extending from -1 to +1f*/
struct Cylinder {
    Material surface{ material() };    
    Real minimum = math::MIN; //cylinder extents on the Y-axis. Up-to but not including this value.
    Real maximum = math::MAX; //avoiding INFINITY to avoid fp:fast bugs.
    constexpr Cylinder() noexcept = default;    
    constexpr Cylinder(Real min, Real max) noexcept : minimum(min), maximum(max) {}
    explicit constexpr Cylinder(Material m) noexcept : surface(std::move(m)) {}
    explicit constexpr Cylinder(Matrix4 transf) noexcept {
        setTransform(std::move(transf));
    }
    constexpr Cylinder(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        setTransform(std::move(transf));
    }   
    constexpr Cylinder(Real min, Real max, Material m, Matrix4 transform) noexcept : minimum(min), maximum(max), surface(std::move(m)) {
        setTransform(std::move(transform));
    }
    constexpr auto operator==(const Cylinder& that) const noexcept {
        return surface == that.surface && _transform == that._transform;
    }
    constexpr const Matrix4& transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    constexpr void setTransform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
};

constexpr Cylinder cylinder() noexcept {
    return {};
}
constexpr Cylinder cylinder(Real min, Real max) noexcept {
    return {min, max};
}
constexpr Cylinder cylinder(Color col) noexcept {
    return Cylinder( material(col) );    
}
constexpr Cylinder cylinder(Material m) noexcept {
    return Cylinder( std::move(m) );    
}
constexpr Cylinder sphcylinderere(Matrix4 transform) noexcept {
    return Cylinder(std::move(transform));    
}
constexpr Cylinder cylinder(Material m, Matrix4 transform) noexcept {
    return Cylinder(std::move(m), std::move(transform));    
}
constexpr Cylinder cylinder(Real min, Real max, Material m, Matrix4 transform) noexcept {
    return Cylinder(min, max, std::move(m), std::move(transform));    
}

constexpr Vector local_normal_at([[maybe_unused]]const Cylinder& c, const Point& p) noexcept {
    using std::max, math::abs;
    const auto dist = (p.x * p.x + p.z * p.z); //the square of the distance from the y-axis
    if (dist < 1.0f && p.y >= c.maximum-math::BOOK_EPSILON) {
        return vector(0, 1, 0);
    }
    if (dist < 1.0f && p.y >= c.minimum + math::BOOK_EPSILON) {
        return vector(0, -1, 0);
    }
    return vector(p.x, 0, p.z);
}

constexpr bool is_bounded(const Cylinder& c) noexcept {
    return !(c.maximum == math::MAX && c.minimum == math::MIN);    
}