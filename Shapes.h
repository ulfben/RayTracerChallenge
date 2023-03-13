#pragma once
#include "pch.h"
#include <variant>
#include "Sphere.h"
#include "Plane.h"

using Shapes = std::variant<Sphere, Plane>;
template<typename T> 
concept shapes = std::is_same_v<Sphere, T> || std::is_same_v<Plane, T>;

constexpr Vector normal_at(const Shapes& variant, const Point& p) { 
    return std::visit([&p](const auto& obj){         
        const auto object_space_point = obj.inv_transform() * p; 
        const auto object_space_normal = local_normal_at(obj, object_space_point);
        auto world_space_normal = transpose(obj.inv_transform()) * object_space_normal;
        world_space_normal.w = 0; //hack to avoid having to work with the transform submatrix
        return normalize(world_space_normal);               
    }, variant);    
}
constexpr bool operator==(const Plane& a, const Shapes& v) noexcept {
    const Plane* b = std::get_if<Plane>(&v);
    return b && *b == a; //true if v contains a T that compares equal to t
}
constexpr bool operator==(const Sphere& a, const Shapes& v) noexcept {
    const Sphere* b = std::get_if<Sphere>(&v);
    return b && *b == a; //true if v contains a T that compares equal to t
}
constexpr bool operator==(const Shapes& v, const Sphere& a) noexcept {
    return a == v;     
}

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
constexpr Material& surface(Shapes& variant) noexcept { 
    return std::visit([](auto& obj) -> Material& { 
        return obj.surface;
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

constexpr const Material& color(const shapes auto& obj) noexcept {
    return obj.surface;
}

constexpr const Matrix4& transform(const shapes auto& obj) noexcept {
    return  obj.transform();
}

constexpr const Matrix4& invTransform(const shapes auto& obj) noexcept {
    return obj.inv_transform();
}

constexpr const Color& color(const shapes auto& obj) noexcept {
    return obj.surface.color;
}

std::ostream& operator<<(std::ostream& os, const Shapes& variant){   
    return std::visit([&os](const auto& val) -> std::ostream& {
        os << val;
        return os;
    }, variant);
}