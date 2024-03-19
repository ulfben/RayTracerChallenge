#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"
#include "Material.h"
#include "Ray.h"
/* unit cylinder, always radius 1, positioned at 0, 0, 0 and extending to infinity on the y axis*/
struct Cylinder final{
    Real minimum = math::MIN; //cylinder extents on the Y-axis. Up-to but not including this value.
    Real maximum = math::MAX; //avoiding INFINITY to avoid fp:fast bugs.
    bool closed = false;
    constexpr Cylinder() noexcept = default;
    constexpr Cylinder(Real min, Real max, bool closed_ = false) noexcept : minimum(min), maximum(max), closed(closed_){}
    explicit constexpr Cylinder(Material m) noexcept : _surface(std::move(m)){}
    explicit constexpr Cylinder(Matrix4 transf) noexcept{
        set_transform(std::move(transf));
    }
    constexpr Cylinder(Material m, Matrix4 transf) noexcept : _surface(std::move(m)){
        set_transform(std::move(transf));
    }
    constexpr Cylinder(Real min, Real max, Material m, Matrix4 transform) noexcept : minimum(min), maximum(max), _surface(std::move(m)){
        set_transform(std::move(transform));
    }
    constexpr Cylinder(Real min, Real max, bool closed_, Material m, Matrix4 transform) noexcept : minimum(min), maximum(max), closed(closed_), _surface(std::move(m)){
        set_transform(std::move(transform));
    }
    constexpr auto operator==(const Cylinder& that) const noexcept{
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

constexpr Cylinder cylinder() noexcept{
    return {};
}
constexpr Cylinder cylinder(Real min, Real max) noexcept{
    return {min, max};
}
constexpr Cylinder cylinder(Color col) noexcept{
    return Cylinder(material(col));
}
constexpr Cylinder cylinder(Material m) noexcept{
    return Cylinder(std::move(m));
}
constexpr Cylinder sphcylinderere(Matrix4 transform) noexcept{
    return Cylinder(std::move(transform));
}
constexpr Cylinder cylinder(Material m, Matrix4 transform) noexcept{
    return Cylinder(std::move(m), std::move(transform));
}
constexpr Cylinder cylinder(Real min, Real max, Material m, Matrix4 transform) noexcept{
    return Cylinder(min, max, std::move(m), std::move(transform));
}
constexpr Cylinder closed_cylinder(Real min, Real max) noexcept{
    return Cylinder(min, max, true);
}
constexpr Cylinder closed_cylinder(Real min, Real max, Material m, Matrix4 transform) noexcept{
    return Cylinder(min, max, true, std::move(m), std::move(transform));
}

//helper to build Icosahedrons
/*constexpr*/ Cylinder closed_cylinder(const Point& p1, const Point& p2, Real radius) noexcept{
    const auto v = p2 - p1;
    auto cyl = closed_cylinder(0, 1);
    auto rot = rotation(vector(0, 1, 0), normalize(v));
    cyl.set_transform(translation(p1) * rot * scaling(radius, magnitude(v), radius));
    return cyl;
}

constexpr Vector local_normal_at([[maybe_unused]] const Cylinder& c, const Point& p) noexcept{
    using math::square;
    const auto dist = square(p.x) + square(p.z); //the square of the distance from the y-axis
    if(dist < 1.0f){
        if(p.y >= (c.maximum - math::BOOK_EPSILON)){
            return vector(0, 1, 0);
        }
        if(p.y <= (c.minimum + math::BOOK_EPSILON)){
            return vector(0, -1, 0);
        }
    }
    return normal_vector(p.x, 0, p.z);
}

constexpr bool is_bounded(const Cylinder& c) noexcept{
    return !(c.maximum == math::MAX && c.minimum == math::MIN);
}

constexpr bool is_closed(const Cylinder& c) noexcept{
    return c.closed;
}
constexpr bool is_open(const Cylinder& c) noexcept{
    return !is_closed(c);
}

constexpr bool check_cap(const Ray& ray, Real t) noexcept{
    using math::square;
    const auto x = ray.x() + t * ray.dx();
    const auto z = ray.z() + t * ray.dz();
    return (square(x) + square(z)) <= 1.0f;
}

constexpr void intersect_caps(const Cylinder& cylinder, const Ray& ray, std::vector<Real>& xs) noexcept{
    if(is_open(cylinder) || math::is_zero(ray.dy())){
        return; //caps only matter if the cylinder is closed and might possibly be intersected by the ray
    }
    const auto t_for_lower_end_cap = (cylinder.minimum - ray.y()) / ray.dy();
    if(check_cap(ray, t_for_lower_end_cap)){ //check ray against the plane at y = cylinder.minimum
        xs.push_back(t_for_lower_end_cap);
    }
    const auto t_for_upper_end_cap = (cylinder.maximum - ray.y()) / ray.dy();
    if(check_cap(ray, t_for_upper_end_cap)){
        xs.push_back(t_for_upper_end_cap);
    }
}

//TODO: refactor this overly long function.
constexpr auto local_intersect([[maybe_unused]] const Cylinder& cylinder, const Ray& local_ray) noexcept{
    using math::square, math::sqrt, math::is_between;
    std::vector<Real> result;
    const auto a = 2 * (square(local_ray.dx()) + square(local_ray.dz()));
    if(!math::is_zero(a, math::BOOK_EPSILON * 2)){  //ray is not ~parallel to the Y axis so we can collide.    
        const auto b = 2.0f * (local_ray.x() * local_ray.dx() + local_ray.z() * local_ray.dz());
        const auto c = square(local_ray.x()) + square(local_ray.z()) - 1.0f;
        const auto discriminant = square(b) - 2.0f * a * c;
        if(discriminant < 0.0f){
            return MISS; //ray does not intersect with the cylinder
        }
        const auto x = -b / a;
        const auto sq = sqrt(discriminant) / a;
        const auto t1 = x - sq;
        const auto t2 = x + sq;
        if(!is_bounded(cylinder)){
            result.push_back(t1);
            result.push_back(t2);
        } else{ //let's compute the height of each intersection
            const auto y1 = local_ray.y() + t1 * local_ray.dy();
            if(is_between(y1, cylinder.minimum, cylinder.maximum)){
                result.push_back(t1);
            }
            const auto y2 = local_ray.y() + t2 * local_ray.dy();
            if(is_between(y2, cylinder.minimum, cylinder.maximum)){
                result.push_back(t2);
            }
        }
    }
    intersect_caps(cylinder, local_ray, result);
    return result;
};