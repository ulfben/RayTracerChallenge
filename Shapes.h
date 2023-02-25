#pragma once
#include "pch.h"
#include <variant>
#include "Sphere.h"

using Shapes = std::variant<Sphere>;

constexpr Vector normal_at(const Shapes& variant, const Point& p) noexcept { 
    return std::visit([&p](const auto& obj){ return local_normal_at(obj, p);}, variant);    
}

template<typename T>
constexpr bool operator==(const T& t, const Shapes& v) noexcept {
    const T* c = std::get_if<T>(&v);
    return c && *c == t; //true if v contains a T that compares equal to t
}
template<typename T>
constexpr bool operator==(const Shapes& v, const T& t) noexcept {
    const T* c = std::get_if<T>(&v);
    return c && *c == t; //true if v contains a T that compares equal to t
}
constexpr bool operator==(const Shapes& lhs, const Shapes& rhs) noexcept {
    return lhs.index() == rhs.index() && std::get<Sphere>(lhs) == std::get<Sphere>(rhs);
}

constexpr Sphere& get_sphere(Shapes& variant){    
    return std::get<Sphere>(variant);
}
constexpr const Sphere& get_sphere(const Shapes& variant){   
    return std::get<Sphere>(variant);
}

std::ostream& operator<<(std::ostream& os, const Shapes& var){
    const auto print_visitor = [&os](const auto& val) -> std::ostream& {
        os << val;
        return os;
    };
    return std::visit(print_visitor, var);
}