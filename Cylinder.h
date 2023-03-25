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
    bool closed = false; 
    constexpr Cylinder() noexcept = default;    
    constexpr Cylinder(Real min, Real max, bool closed_ = false) noexcept : minimum(min), maximum(max), closed(closed_) {}
    explicit constexpr Cylinder(Material m) noexcept : surface(std::move(m)) {}
    explicit constexpr Cylinder(Matrix4 transf) noexcept {
        set_transform(std::move(transf));
    }
    constexpr Cylinder(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        set_transform(std::move(transf));
    }   
    constexpr Cylinder(Real min, Real max, Material m, Matrix4 transform) noexcept : minimum(min), maximum(max), surface(std::move(m)) {
        set_transform(std::move(transform));
    }
    constexpr auto operator==(const Cylinder& that) const noexcept {
        return surface == that.surface && _transform == that._transform;
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
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
constexpr Cylinder closed_cylinder(Real min, Real max) noexcept {
    return {min, max, true};
}

constexpr Vector local_normal_at([[maybe_unused]]const Cylinder& c, const Point& p) noexcept {
    using std::max, math::abs, math::square;
    const auto dist = square(p.x) + square(p.z); //the square of the distance from the y-axis
    if (dist < 1.0f) {
        if (p.y >= c.maximum - math::BOOK_EPSILON) {
            return vector(0, 1, 0);
        }
        if (p.y >= c.minimum + math::BOOK_EPSILON) {
            return vector(0, -1, 0);
        }
    }
    return normalize(vector(p.x, 0, p.z));
}

constexpr bool is_bounded(const Cylinder& c) noexcept {
    return !(c.maximum == math::MAX && c.minimum == math::MIN);    
}

constexpr bool is_closed(const Cylinder& c) noexcept {
    return c.closed;    
}
constexpr bool is_open(const Cylinder& c) noexcept {
    return !is_closed(c);    
}

constexpr auto local_intersect([[maybe_unused]] const Cylinder& cylinder, Ray local_ray) noexcept {
    using math::square, math::sqrt, math::is_between;
    const auto a = square(local_ray.direction.x) + square(local_ray.direction.z);
    if (a < math::BOOK_EPSILON) { //close to 0
        return MISS; //ray is ~parallel to the Y axis so we can't collide.
    }
    const auto b = 2.0f * local_ray.origin.x * local_ray.direction.x +
        2.0f * local_ray.origin.z * local_ray.direction.z;
    const auto c = square(local_ray.origin.x) + square(local_ray.origin.z) - 1.0f;
    const auto discriminant = square(b) - 4.0f * a * c;
    if (discriminant < 0.0f) {
        return MISS; //ray does not intersect with the cylinder
    }
    auto t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    auto t2 = (-b + sqrt(discriminant)) / (2.0f * a);
    if (t1 > t2) {
        std::swap(t1, t2);
    }
    if (!is_bounded(cylinder)) {
        return std::vector{ t1, t2 };
    }
    std::vector<Real> result;
     //let's compute the height of each intersection
    const auto y1 = local_ray.origin.y + t1 * local_ray.direction.y;
    if (is_between(y1, cylinder.minimum, cylinder.maximum)) {
        result.push_back(t1); 
    }
    const auto y2 = local_ray.origin.y + t2 * local_ray.direction.y;
    if (is_between(y2, cylinder.minimum, cylinder.maximum)) {
        result.push_back(t2); 
    }    
    return result;
};