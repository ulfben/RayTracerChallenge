#pragma once
#include "pch.h"
#include "Matrix.h"
#include "Shapes.h"

struct Group final{    
    constexpr bool empty() const noexcept { return _shapes.empty(); }
    constexpr size_t size() const noexcept { return _shapes.size(); }

    constexpr auto operator==(const Group& that) const noexcept {
        return _shapes == that._shapes && _transform == that._transform;
    }
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
    std::vector<Shapes> _shapes;
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
};

constexpr Vector local_normal_at([[maybe_unused]]const Group& g, const Point& object_space_point) noexcept { 
    return normalize(vector(object_space_point)); /*imagine object_space_point - s.position, but s position is always 0*/    
}