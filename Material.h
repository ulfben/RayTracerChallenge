#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Pattern.h"

struct IoR { //index of refraction
    constexpr static auto vacuum = 1.0f;
    constexpr static auto air = 1.00029f;
    constexpr static auto water = 1.333f;
    constexpr static auto ice = 1.31f;
    constexpr static auto plexiglass = 1.49f;
    constexpr static auto glass = 1.52f;
    constexpr static auto diamond = 2.417f;
};

struct Material final {
    Patterns pattern = null_pattern();
    Color color{ WHITE };
    Real ambient = 0.1f;
    Real diffuse = 0.9f;
    Real specular = 0.9f;
    Real shininess = 200.0f;
    Real reflective = 0.0f;
    Real transparency = 0.0f;
    Real refractive_index = IoR::vacuum;
    
    constexpr Material() noexcept = default;
    constexpr Material(Color color_, Real ambient_ = 0.1f, Real diffuse_ = 0.9f, Real specular_ = 0.9f) noexcept : 
        color{ color_ }, ambient{ ambient_ }, diffuse{ diffuse_ }, specular{specular_} {
    }
    constexpr explicit Material(Patterns pattern_) noexcept : pattern{ std::move(pattern_) } {}    
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
    return Material(c, ambient, diffuse, specular);    
}
constexpr Material mirror() noexcept {
    Material m = material();
    m.reflective = 1.0f;
    return m;
}
constexpr Material glass(Real refractive = IoR::glass) noexcept {
    Material m = material();
    m.transparency = 1.0f;
    m.refractive_index = refractive;
    return m;
}
constexpr Material material(Patterns pattern) noexcept {    
    return Material(std::move(pattern)); 
}

constexpr Material material(Material mat, const Matrix4& texture_transform) noexcept {    
    auto& current_transform = transform(mat.pattern);
    set_transform(mat.pattern, current_transform * texture_transform);
    return mat; 
}

constexpr bool has_pattern(const Material& mat) noexcept {
    return std::visit([](const auto& obj) noexcept -> bool { return static_cast<bool>(obj); }, mat.pattern);
}