#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"
/*A unit AABB, always positioned at 0, 0, 0 and extending from -1 to +1f*/
struct Cube final{
    constexpr Cube() noexcept = default;
    explicit constexpr Cube(Material m) noexcept : _surface(std::move(m)){}
    explicit constexpr Cube(Matrix4 transf) noexcept{
        set_transform(std::move(transf));
    }
    constexpr Cube(Material m, Matrix4 transf) noexcept : _surface(std::move(m)){
        set_transform(std::move(transf));
    }
    constexpr auto operator==(const Cube& that) const noexcept{
        return _surface == that._surface && _transform == that._transform;
    }
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
private:
    Material _surface{material()};
    Matrix4 _transform{Matrix4Identity};
    Matrix4 _invTransform{Matrix4Identity};
};

constexpr Cube cube() noexcept{
    return {};
}
constexpr Cube cube(Color col) noexcept{
    return Cube(material(col));
}
constexpr Cube cube(Material m) noexcept{
    return Cube(std::move(m));
}
constexpr Cube cube(Matrix4 transform) noexcept{
    return Cube(std::move(transform));
}
constexpr Cube cube(Material m, Matrix4 transform) noexcept{
    return Cube(std::move(m), std::move(transform));
}

constexpr Vector local_normal_at([[maybe_unused]] const Cube& c, const Point& p) noexcept{
    using std::max, math::abs;
    const auto max_component = max({abs(p.x), abs(p.y), abs(p.z)});
    if(max_component == abs(p.x)){
        return vector(p.x, 0, 0);
    }
    if(max_component == abs(p.y)){
        return vector(0, p.y, 0);
    }
    return vector(0, 0, p.z);
}

constexpr std::pair<Real, Real> check_axis(Real origin, Real direction, Real min = -1.0f, Real max = 1.0f) noexcept{
    const auto tmin_numerator = (min - origin);
    const auto tmax_numerator = (max - origin);
    Real tmin, tmax;
    if(math::abs(direction) >= math::BOOK_EPSILON){
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    } else{
        tmin = tmin_numerator * math::MAX; //arbitrary large-ish value. used to be INFINITY, but that breaks when compiled
        tmax = tmax_numerator * math::MAX; //with /fp:fast. I just multiply with *something* to keep the signs correct.
    }
    return (tmin > tmax) ? std::pair{tmax, tmin} : std::pair{tmin, tmax};
}

constexpr auto local_intersect([[maybe_unused]] const Cube& cube, const Ray& local_ray) noexcept{
    const auto [xtmin, xtmax] = check_axis(local_ray.x(), local_ray.dx());
    const auto [ytmin, ytmax] = check_axis(local_ray.y(), local_ray.dy());
    const auto [ztmin, ztmax] = check_axis(local_ray.z(), local_ray.dz());
    const auto tmin = math::max(xtmin, ytmin, ztmin);
    const auto tmax = math::min(xtmax, ytmax, ztmax);
    if(tmin > tmax) return MISS;
    return  std::vector{tmin, tmax};
};
