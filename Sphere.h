#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"

struct Sphere final {
    Matrix4 transform{ Matrix4Identity }; 
    Material surface{ material() }; 
    Point position{0,0,0};
    Real radius{1.0f};    
    constexpr Sphere() noexcept = default;
    constexpr Sphere(Point p, Real rad) noexcept : position(p), radius(rad)
    {}
    constexpr Sphere(Point p, Real rad, Material m) noexcept : surface(std::move(m)), position(p), radius(rad)
    {}
    explicit constexpr Sphere(Material m) noexcept : surface(std::move(m))
    {}
    explicit constexpr Sphere(Matrix4 transf) noexcept : transform(std::move(transf))
    {}
    constexpr auto operator==(const Sphere& that) const noexcept {
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
constexpr Sphere sphere(Matrix4 transform) noexcept {
    return Sphere(std::move(transform));    
}
constexpr Sphere sphere() noexcept {
    return Sphere{ point(0,0,0), 1.0f };
}

#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Sphere& t) {
    os << std::format("Sphere({}, {})"sv, t.position, t.radius);
    return os;
}
#pragma warning(pop)

constexpr Vector local_normal_at([[maybe_unused]]const Sphere& s, const Point& object_space_point) noexcept { 
    return object_space_point; // -s.position; /*s position is always 0*/    
}
