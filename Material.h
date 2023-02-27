#pragma once
#include "pch.h"
#include "Tuple.h"

struct Material final {
    Color color{ 1, 1, 1 };
    Real ambient = 0.1f;
    Real diffuse = 0.9f; 
    Real specular = 0.9f;
    Real shininess = 200.0f;
    constexpr bool operator==(const Material& that) const noexcept = default;
};
#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Material& t) {
    os << std::format("Material(color({},{},{}), am({:f}), di({:f}), sp({:f}), sh({:f})"sv, t.color.r, t.color.g, t.color.b, t.ambient, t.diffuse, t.specular, t.shininess);
    return os;
}
#pragma warning(pop)
constexpr Material material() noexcept {
    return {};
}
constexpr Material material(Color c, Real ambient = 0.1f, Real diffuse = 0.9f, Real specular = 0.9f, Real shininess = 200.0f) noexcept {
    return Material{c, ambient, diffuse, specular, shininess};    
}