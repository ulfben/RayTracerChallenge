#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"

struct Cone {
    Material surface{ material() };    
    Real minimum = math::MIN; //cone extents on the Y-axis. Up-to but not including this value.
    Real maximum = math::MAX; //avoiding INFINITY to avoid fp:fast bugs.
    bool closed = false; 
    constexpr Cone() noexcept = default;    
    constexpr Cone(Real min, Real max, bool closed_ = false) noexcept : minimum(min), maximum(max), closed(closed_) {}
    explicit constexpr Cone(Material m) noexcept : surface(std::move(m)) {}
    explicit constexpr Cone(Matrix4 transf) noexcept {
        set_transform(std::move(transf));
    }
    constexpr Cone(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        set_transform(std::move(transf));
    }   
    constexpr Cone(Real min, Real max, Material m, Matrix4 transform) noexcept : minimum(min), maximum(max), surface(std::move(m)) {
        set_transform(std::move(transform));
    }
    constexpr Cone(Real min, Real max, bool closed_, Material m, Matrix4 transform) noexcept : minimum(min), maximum(max), closed(closed_), surface(std::move(m)) {
        set_transform(std::move(transform));
    }
    constexpr auto operator==(const Cone& that) const noexcept {
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

constexpr Cone cone() noexcept {
    return {};
}
constexpr Cone cone(Real min, Real max) noexcept {
    return {min, max};
}
constexpr Cone cone(Color col) noexcept {
    return Cone( material(col) );    
}
constexpr Cone cone(Material m) noexcept {
    return Cone( std::move(m) );    
}
constexpr Cone sphconeere(Matrix4 transform) noexcept {
    return Cone(std::move(transform));    
}
constexpr Cone cone(Material m, Matrix4 transform) noexcept {
    return Cone(std::move(m), std::move(transform));    
}
constexpr Cone cone(Real min, Real max, Material m, Matrix4 transform) noexcept {
    return Cone(min, max, std::move(m), std::move(transform));    
}
constexpr Cone closed_cone(Real min, Real max) noexcept {
    return Cone(min, max, true);
}
constexpr Cone closed_cone(Real min, Real max, Material m, Matrix4 transform) noexcept {
    return Cone(min, max, true, std::move(m), std::move(transform));    
}

constexpr Vector local_normal_at([[maybe_unused]]const Cone& c, const Point& p) noexcept {
    using math::square;
    const auto dist = square(p.x) + square(p.z); //the square of the distance from the y-axis
    if (dist < 1.0f) {
        if (p.y >= (c.maximum - math::BOOK_EPSILON)) {
            return vector(0, 1, 0);
        }
        if (p.y <= (c.minimum + math::BOOK_EPSILON)) {
            return vector(0, -1, 0);
        }
    }
    auto y = math::sqrt(dist);
    if (p.y > 0){
        y *= -1;
    }
    return normal_vector(p.x, y, p.z);
}

constexpr bool is_bounded(const Cone& c) noexcept {
    return !(c.maximum == math::MAX && c.minimum == math::MIN);    
}

constexpr bool is_closed(const Cone& c) noexcept {
    return c.closed;    
}
constexpr bool is_open(const Cone& c) noexcept {
    return !is_closed(c);    
}

// same as the cylinder check_cap
constexpr bool check_cap(const Ray& ray, Real t, Real y) noexcept {
    using math::square;
    const auto x = ray.x() + t * ray.dx();
    const auto z = ray.z() + t * ray.dz();
    return (square(x) + square(z)) <= square(y);
}

constexpr void intersect_caps(const Cone& cone, const Ray& ray, std::vector<Real>& xs) noexcept {
    if (is_open(cone) || math::is_zero(ray.dy())) {
        return; //caps only matter if the cone is closed and might possibly be intersected by the ray
    }
    const auto t_for_lower_end_cap = (cone.minimum - ray.y()) / ray.dy();
    if (check_cap(ray, t_for_lower_end_cap, cone.minimum)) { //check ray against the plane at y = cone.minimum
       xs.push_back(t_for_lower_end_cap);
    }
    const auto t_for_upper_end_cap = (cone.maximum - ray.y()) / ray.dy();
    if (check_cap(ray, t_for_upper_end_cap, cone.maximum)) {
       xs.push_back(t_for_upper_end_cap);
    }
}

//TODO: refactor this overly long function.
constexpr auto local_intersect([[maybe_unused]] const Cone& cone, const Ray& local_ray) noexcept {
    using math::square, math::is_zero, math::sqrt, math::is_between, math::max, math::abs;
    std::vector<Real> result;
    const auto a = square(local_ray.dx()) - square(local_ray.dy()) + square(local_ray.dz());   
    const auto b = 2 * ((local_ray.x() * local_ray.dx()) - (local_ray.y() * local_ray.dy()) + (local_ray.z() * local_ray.dz()));
    const auto c = square(local_ray.x()) - square(local_ray.y()) + square(local_ray.z());
    
    //When a is zero, it means the ray is parallel to one of the cone’s halves
    //this still means the ray might intersect the other half of the cone.
    if (is_zero(a)){  //In this case the ray will miss when both a and b are zero.
        if (!is_zero(b)) { //If a is zero but b isn’t, calc the single point of intersection:
            const auto t = -c / (2.0f * b);
            result.push_back(t);
        }
    }    
    else {// a is non-zero
        //In general, the discriminant of the quadratic equation should be non-negative if there are real solutions, 
        // and negative if there are complex solutions. However, due to floating-point roundoff errors, the discriminant 
        // may sometimes become slightly negative even when there are real solutions, leading to incorrect results.
        //Here, rel_eps is a small relative epsilon based on the magnitudes of the coefficients, making it more robust to different combinations of coefficients 
        const auto rel_eps = math::GTEST_EPSILON * max(abs(a), abs(b), abs(c));
        const auto discriminant = square(b) - 4.0f * a * c;
        if (discriminant >= -rel_eps) { //The discriminant is considered to be non-negative if it is greater than or equal to -rel_eps            
            const auto sqrt_discriminant = sqrt(max(discriminant, 0.0f)); //round to 0 if need be.
            const auto t1 = (-b - sqrt_discriminant) / (2 * a);
            const auto t2 = (-b + sqrt_discriminant) / (2 * a);                  
            const auto y1 = local_ray.y() + t1 * local_ray.dy();
            if (is_between(y1, cone.minimum, cone.maximum)) {
                result.push_back(t1);
            }
            const auto y2 = local_ray.y() + t2 * local_ray.dy();
            if (is_between(y2, cone.minimum, cone.maximum)) {
                result.push_back(t2);
            }            
        }
    }
    intersect_caps(cone, local_ray, result);
    return result;
};