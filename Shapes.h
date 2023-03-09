#pragma once
#include "pch.h"
#include <variant>
#include "Sphere.h"
#include "Plane.h"

using Shapes = std::variant<Sphere, Plane>;

constexpr Vector normal_at(const Shapes& variant, const Point& p) { 
    return std::visit([&p](const auto& obj){ 
        const auto inv_transform = inverse(obj.transform);
        const auto object_space_point = inv_transform * p; 
        const auto object_space_normal = local_normal_at(obj, object_space_point);
        auto world_space_normal = transpose(inv_transform) * object_space_normal;
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

constexpr Matrix4& transform(Shapes& variant) noexcept { 
    return std::visit([](auto& obj) -> Matrix4& { 
        return obj.transform;
    }, variant);    
}
constexpr Material& surface(Shapes& variant) noexcept { 
    return std::visit([](auto& obj) -> Material& { 
        return obj.surface;
    }, variant);    
}

constexpr const Matrix4& transform(const Shapes& variant) noexcept { 
    return std::visit([](const auto& obj) -> const Matrix4& { 
        return obj.transform;
    }, variant);    
}
constexpr const Material& surface(const Shapes& variant) noexcept { 
    return std::visit([](const auto& obj) -> const Material& { 
        return obj.surface;
    }, variant);    
}


//constexpr Sphere& get_sphere(Shapes& variant){    
//    return std::get<Sphere>(variant);
//}
//constexpr const Sphere& get_sphere(const Shapes& variant){   
//    return std::get<Sphere>(variant);
//}

std::ostream& operator<<(std::ostream& os, const Shapes& var){
    const auto print_visitor = [&os](const auto& val) -> std::ostream& {
        os << val;
        return os;
    };
    return std::visit(print_visitor, var);
}