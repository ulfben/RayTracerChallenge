#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"
/*A plane is perfectly flat and extends infinitely on the x and z dimensions. It is infinitely thin on the y axis.
It's normal is the same at every point. */
struct Plane final {
    Material surface{};     
    constexpr Plane() noexcept = default;        
    explicit constexpr Plane(Material m) noexcept : surface(std::move(m))
    {}
    explicit constexpr Plane(Matrix4 transf) noexcept {
        set_transform(std::move(transf));
    }
    constexpr Plane(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        set_transform(std::move(transf));
    }

    constexpr auto operator==(const Plane& that) const noexcept {
        return surface == that.surface && _transform == that._transform;
    };
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

constexpr Plane plane() noexcept {
    return Plane{};
}
constexpr Plane plane(Matrix4 transform) noexcept {
    return Plane(std::move(transform));
}
constexpr Plane plane(Material surface) noexcept {
    return Plane(std::move(surface));
}

constexpr Plane plane(Color col) noexcept {
    return Plane(material(col));
}
constexpr Plane plane(Material surface, Matrix4 transform) noexcept {
    return Plane(std::move(surface), std::move(transform));
}

constexpr Vector local_normal_at([[maybe_unused]]const Plane& s, [[maybe_unused]] const Point& local_point) noexcept {     
    return vector(0.0f, 1.0f, 0.0f);
}

constexpr auto local_intersect([[maybe_unused]] const Plane& p, const Ray& local_ray) {
    if (math::abs(local_ray.direction.y) < math::BOOK_EPSILON) {
        return MISS;
    }
    const auto t1 = -local_ray.origin.y / local_ray.direction.y;
    return std::vector{ t1 };
};