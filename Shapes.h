#pragma once
#include "pch.h"
#include <variant>
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Cylinder.h"

#include "Matrix.h"
#include "Color.h"
#include "Material.h"

using Shapes = std::variant<Sphere, Plane, Cube, Cylinder>;
template<typename T> 
concept is_shape = std::is_same_v<Sphere, T> || std::is_same_v<Plane, T> || std::is_same_v<Cube, T> || std::is_same_v<Cylinder, T>;

constexpr Vector normal_at(const Shapes& variant, const Point& p) { 
    return std::visit([&p](const auto& obj){         
        const auto object_space_point = obj.inv_transform() * p; 
        const auto object_space_normal = local_normal_at(obj, object_space_point);
        auto world_space_normal = transpose(obj.inv_transform()) * object_space_normal;
        world_space_normal.w = 0; //hack to avoid having to work with the transform submatrix
        return normalize(world_space_normal);               
    }, variant);    
}
template<typename T>
requires is_shape<T>
constexpr bool operator==(const T& t, const Shapes& v) {
    const T* c = std::get_if<T>(&v);
    return c && *c == t; // true if v contains a pattern that compares equal to v
}

constexpr bool operator==(const Patterns& v, const is_shape auto& t) {
    return t == v;
};

constexpr const Matrix4& transform(const Shapes& variant) noexcept { 
    return std::visit([](const auto& obj) -> const Matrix4& { 
        return obj.transform();
    }, variant);    
}
constexpr const Matrix4& invTransform(const Shapes& variant) noexcept { 
    return std::visit([](const auto& obj) -> const Matrix4& { 
        return obj.inv_transform();
    }, variant);    
}
constexpr const Material& surface(const Shapes& variant) noexcept { 
    return std::visit([](const auto& obj) -> const Material& { 
        return obj.surface;
    }, variant);    
}
constexpr const Color& color(const Shapes& variant) noexcept { 
    return std::visit([](const auto& obj) -> const Color& { 
        return obj.surface.color;
    }, variant);    
}

constexpr const Material& color(const is_shape auto& obj) noexcept {
    return obj.surface;
}

constexpr const Matrix4& transform(const is_shape auto& obj) noexcept {
    return  obj.transform();
}

constexpr const Matrix4& invTransform(const is_shape auto& obj) noexcept {
    return obj.inv_transform();
}

constexpr const Color& color(const is_shape auto& obj) noexcept {
    return obj.surface.color;
}

std::ostream& operator<<(std::ostream& os, const Shapes& variant){   
    return std::visit([&os](const auto& val) -> std::ostream& {
        os << val;
        return os;
    }, variant);
}