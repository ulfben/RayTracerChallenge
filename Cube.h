#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"

struct Cube {
    Material surface{ material() };          
    constexpr Cube() noexcept = default;    
    explicit constexpr Cube(Material m) noexcept : surface(std::move(m)) {}
    explicit constexpr Cube(Matrix4 transf) noexcept {
        setTransform(std::move(transf));
    }
    constexpr Cube(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        setTransform(std::move(transf));
    }   
    constexpr auto operator==(const Cube& that) const noexcept {
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

constexpr Cube cube() noexcept {
    return {};
}

constexpr Vector local_normal_at([[maybe_unused]]const Cube& c, const Point& p) noexcept {
    using std::max, math::abs;
    const auto max_component = max({ abs(p.x), abs(p.y), abs(p.z) });
    if (max_component == abs(p.x)) {
        return vector(p.x, 0, 0);
    }
    if (max_component == abs(p.y)) {
        return vector(0, p.y, 0);
    }
    return vector(0, 0, p.z);
}

constexpr std::pair<Real, Real> check_axis(Real origin, Real direction) noexcept {
    const auto tmin_numerator = (-1 - origin);
    const auto tmax_numerator = (1 - origin);
    Real tmin, tmax; 
    if (math::abs(direction) >= math::BOOK_EPSILON) {
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    }
    else {
        tmin = tmin_numerator * INFINITY;
        tmax = tmax_numerator * INFINITY;
    }
    if (tmin > tmax) {
        std::swap(tmin, tmax);
    }
    return { tmin, tmax };
}

