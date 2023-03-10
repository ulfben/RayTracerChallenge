#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Pattern.h"

struct Material final {
    Color color{ 1, 1, 1 };
    Real ambient = 0.1f;
    Real diffuse = 0.9f;
    Real specular = 0.9f;
    Real shininess = 200.0f;
    Real reflective = 0.0f;
    Real transparency = 0.0f;
    Real refractive_index = 0.0f;
    Patterns pattern = null_pattern();
    constexpr bool operator==(const Material& that) const noexcept = default;
};
#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Material& t) {
    os << std::format("Material(color({},{},{}), am({:f}), di({:f}), sp({:f}), sh({:f}, refl({:f}), trans({:f}), refract_i({:f}))"sv, t.color.r, t.color.g, t.color.b, t.ambient, t.diffuse, t.specular, t.shininess, t.reflective, t.transparency, t.refractive_index);
    return os;
}
#pragma warning(pop)
constexpr Material material() noexcept {
    return {};
}
constexpr Material material(Color c, Real ambient = 0.1f, Real diffuse = 0.9f, Real specular = 0.9f) noexcept {
    return Material{c, ambient, diffuse, specular};    
}
constexpr Material mirror() noexcept {
    Material m{};
    m.reflective = 1.0f;
    return m;
}
constexpr Material glass(Real refractive = 1.52f) noexcept {
    Material m{};
    m.transparency = 1.0f;
    m.refractive_index = refractive;
    return m;
}

constexpr bool has_pattern(const Material& mat) noexcept {
    return std::visit([](const auto& obj) noexcept -> bool { return static_cast<bool>(obj); }, mat.pattern);

}