#pragma once
#include "pch.h"
#include "Matrix.h"
#include "Shapes.h"

struct Group final{
    constexpr bool empty() const noexcept{ return _shapes.empty(); }
    constexpr size_t size() const noexcept{ return _shapes.size(); }

    constexpr auto operator==(const Group& that) const noexcept{
        return _shapes == that._shapes && _transform == that._transform;
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
    std::vector<Shapes> _shapes;
};

constexpr Vector local_normal_at([[maybe_unused]] const Group& g, const Point& object_space_point) noexcept{
    return normalize(vector(object_space_point)); /*imagine object_space_point - s.position, but s position is always 0*/
}

constexpr auto local_intersect([[maybe_unused]] const Group& g, [[maybe_unused]] const Ray& local_ray) noexcept{
    return std::vector<Real>();
}