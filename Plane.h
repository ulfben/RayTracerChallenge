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
        setTransform(std::move(transf));
    }
    constexpr Plane(Material m, Matrix4 transf) noexcept : surface(std::move(m)) {
        setTransform(std::move(transf));
    }

    constexpr auto operator==(const Plane& that) const noexcept {
        return surface == that.surface && transform == that.transform;
    };
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

constexpr Plane plane() noexcept {
    return Plane{};
}
constexpr Plane plane(Matrix4 transform) noexcept {
    return Plane(std::move(transform));
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
