#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"

/*A unit Sphere, always positioned at 0, 0, 0 and with a radius of 1.0f*/
struct Sphere final { 
    Material surface{ material() };          
    constexpr Sphere() noexcept = default;
    explicit constexpr Sphere(Material m) noexcept : surface(std::move(m))
    {}
    explicit constexpr Sphere(Matrix4 transf) noexcept {
        set_transform(std::move(transf));
    }
    constexpr Sphere(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        set_transform(std::move(transf));
    }
    constexpr auto operator==(const Sphere& that) const noexcept {
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

constexpr Sphere sphere() noexcept {
    return Sphere{};
}
constexpr Sphere sphere(Color col) noexcept {
    return Sphere( material(col) );    
}
constexpr Sphere sphere(Material m) noexcept {
    return Sphere( std::move(m) );    
}
constexpr Sphere sphere(Matrix4 transform) noexcept {
    return Sphere(std::move(transform));    
}
constexpr Sphere sphere(Material m, Matrix4 transform) noexcept {
    return Sphere(std::move(m), std::move(transform));    
}

#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Sphere& t) {
    os << std::format("Sphere({})"sv, t.get_transform()); 
    return os;
}
#pragma warning(pop)

constexpr Vector local_normal_at([[maybe_unused]]const Sphere& s, const Point& object_space_point) noexcept { 
    return normalize(vector(object_space_point)); /*imagine object_space_point - s.position, but s position is always 0*/    
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
constexpr auto local_intersect([[maybe_unused]] const Sphere& s, const Ray& local_ray) {
    using math::square, math::sqrt;
    constexpr Real SPHERE_RADIUS = 1.0f; //unit sphere, always at 0, 0, 0 and radius 1. 
    const auto  a = 2*(square(local_ray.dx()) + square(local_ray.dy()) + square(local_ray.dz()));
    const auto  b = 2 * (local_ray.dx() * local_ray.x() + local_ray.dy() * local_ray.y() + local_ray.dz() * local_ray.z()); 
    const auto  c = square(local_ray.x()) + square(local_ray.y()) + square(local_ray.z()) - SPHERE_RADIUS;
    const auto  discriminant = square(b) - 2 * a * c;
    if (discriminant < 0) {
        return MISS;
    }    
    const auto x1 = -b/a;
    const auto sqrtDet = sqrt(discriminant)/a;
    const auto t1 = x1 - sqrtDet;
    const auto t2 = x1 + sqrtDet;
    return std::vector{ t1, t2 };
};