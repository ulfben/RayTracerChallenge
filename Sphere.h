#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"

struct Sphere final { 
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
    explicit constexpr Sphere(Matrix4 transf) noexcept {
        setTransform(std::move(transf));
    }
    constexpr Sphere(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        setTransform(std::move(transf));
    }
    constexpr Sphere(Point p, Real rad, Matrix4 transf) noexcept : position(p), radius(rad) {
        setTransform(std::move(transf));
    }
    constexpr auto operator==(const Sphere& that) const noexcept {
        return position == that.position && math::float_cmp(radius, that.radius)
            && surface == that.surface && transform == that.transform;
    }
    constexpr const Matrix4& getTransform() const noexcept {
        return transform;
    }
    constexpr const Matrix4& getInvTransform() const noexcept {
        return invTransform;
    }
    constexpr void setTransform(Matrix4 mat) noexcept {
        transform = std::move(mat);
        invTransform = inverse(transform);
    }
private: 
    Matrix4 transform{ Matrix4Identity };
    Matrix4 invTransform{ Matrix4Identity };
};

constexpr Sphere sphere(Point p, Real radius) noexcept {
    return Sphere( p, radius );
}
constexpr Sphere sphere(Point p, Real radius, Matrix4 transform) noexcept {
    return Sphere( p, radius, std::move(transform) );
}
constexpr Sphere sphere(Color col) noexcept {
    return Sphere( ORIGO, 1.0f, material(col) );    
}
constexpr Sphere sphere(Material m) noexcept {
    return Sphere( ORIGO, 1.0f, std::move(m) );    
}
constexpr Sphere sphere(Matrix4 transform) noexcept {
    return Sphere(std::move(transform));    
}
constexpr Sphere sphere(Material m, Matrix4 transform) noexcept {
    return Sphere(std::move(m), std::move(transform));    
}
constexpr Sphere sphere() noexcept {
    return Sphere( ORIGO, 1.0f );
}

#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Sphere& t) {
    os << std::format("Sphere({}, {})"sv, t.position, t.radius);
    return os;
}
#pragma warning(pop)

constexpr Vector local_normal_at([[maybe_unused]]const Sphere& s, const Point& object_space_point) noexcept { 
    return normalize(object_space_point-s.position); /*s position is always 0*/    
}
