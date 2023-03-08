#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"
/*A plane is perfectly flat and extends infinitely on the x and z dimensions. It is infinitely thin on the y axis.
It's normal is the same at every point. */
struct Plane final {
    Matrix4 transform{ Matrix4Identity }; 
    Material surface{};             
    constexpr auto operator==(const Plane& that) const noexcept {
        return surface == that.surface && transform == that.transform;
    }
};

constexpr Plane plane() noexcept {
    return Plane{};
}

constexpr Vector local_normal_at([[maybe_unused]]const Plane& s, [[maybe_unused]] const Point& local_point) noexcept {     
    return vector(0.0f, 1.0f, 0.0f);
}
