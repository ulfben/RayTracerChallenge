#pragma once
#include "pch.h"
#include <variant>
#include "Sphere.h"
#include "Plane.h"

using Shapes = std::variant<Sphere, Plane>;

constexpr Vector normal_at(const Shapes& variant, const Point& p) { 
    return std::visit([&p](const auto& obj){ return local_normal_at(obj, p);}, variant);    
}

constexpr bool operator==(const Sphere& a, const Shapes& v) noexcept {
    const Sphere* b = std::get_if<Sphere>(&v);
    return b && *b == a; //true if v contains a T that compares equal to t
}
constexpr bool operator==(const Shapes& v, const Sphere& a) noexcept {
    return a == v;     
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