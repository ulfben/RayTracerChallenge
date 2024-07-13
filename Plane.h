#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"
struct Group;
/*A plane is perfectly flat and extends infinitely on the x and z dimensions. It is infinitely thin on the y axis.
It's normal is the same at every point. */
struct Plane final{
    constexpr Plane() noexcept = default;
    explicit constexpr Plane(Material m) noexcept : _surface(std::move(m)){}
    explicit constexpr Plane(Matrix4 transf) noexcept{
        set_transform(std::move(transf));
    }
    constexpr Plane(Material m, Matrix4 transf) noexcept : _surface(std::move(m)){
        set_transform(std::move(transf));
    }

    constexpr auto operator==(const Plane& that) const noexcept{
        return _surface == that._surface && _transform == that._transform;
    };
    constexpr const Matrix4& get_transform() const noexcept{
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept{
        return _invTransform;
    }
    constexpr void set_transform(Matrix4 mat) noexcept{
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Material& surface() const noexcept{
        return _surface;
    }
    constexpr const Color& color() const noexcept{
        return surface().color;
    }
    constexpr Material& surface() noexcept{
        return _surface;
    }
    constexpr Color& color() noexcept{
        return surface().color;
    }
    constexpr void set_parent(Group* g) noexcept{
        _parent = g;
    }
    constexpr Group* get_parent() const noexcept{
        return _parent;
    }
private:
    Material _surface{};
    Matrix4 _transform{Matrix4Identity};
    Matrix4 _invTransform{Matrix4Identity};
    Group* _parent = nullptr;
};

constexpr Plane plane() noexcept{
    return Plane{};
}
constexpr Plane plane(Matrix4 transform) noexcept{
    return Plane(std::move(transform));
}
constexpr Plane plane(Material surface) noexcept{
    return Plane(std::move(surface));
}

constexpr Plane plane(Color col) noexcept{
    return Plane(material(col));
}
constexpr Plane plane(Material surface, Matrix4 transform) noexcept{
    return Plane(std::move(surface), std::move(transform));
}

constexpr Vector local_normal_at([[maybe_unused]] const Plane& s, [[maybe_unused]] const Point& local_point) noexcept{
    return vector(0.0f, 1.0f, 0.0f);
}

constexpr auto local_intersect([[maybe_unused]] const Plane& p, const Ray& local_ray){
    if(math::abs(local_ray.dy()) < math::BOOK_EPSILON){
        return MISS;
    }
    const auto t1 = -local_ray.y() / local_ray.dy();
    return std::vector{t1};
};