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
        setTransform(std::move(transf));
    }
    constexpr Sphere(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        setTransform(std::move(transf));
    }
    constexpr auto operator==(const Sphere& that) const noexcept {
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
    os << std::format("Sphere({})"sv, t.transform()); 
    return os;
}
#pragma warning(pop)

constexpr Vector local_normal_at([[maybe_unused]]const Sphere& s, const Point& object_space_point) noexcept { 
    return normalize(object_space_point/*-s.position*/); /*s position is always 0*/    
}
