#pragma once
#include "pch.h"
#include "Tuple.h"

struct Light final {    
    Point position;
    Color intensity;
};

constexpr Light point_light(Point p, Color i) noexcept {
    return Light{ p, i };
}

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

constexpr Color lighting(const Material& surface, const Light& light, const Point& p, const Vector& eye, const Vector& normal, bool in_shadow = false) noexcept {
    const auto effective_color = surface.color * light.intensity;
    const auto direction_to_light = normalize(light.position - p);
    const auto ambient = effective_color * surface.ambient;
    const auto light_dot_normal = dot(direction_to_light, normal);
    if (light_dot_normal <= 0 || in_shadow) {
        return ambient; //light dot normal, if negative, means the light is on the opposite side of the surface, so both the diffuse and specular component will be 0
    }
    const auto diffuse = effective_color * surface.diffuse * light_dot_normal;
    const auto reflection_v = reflect(-direction_to_light, normal);
    //reflection vector dot eye vector, if negative, means the light reflects away from the eye, so the specular component will be 0.
    const auto specular = light.intensity * surface.specular * std::pow(std::max(dot(reflection_v, eye), 0.0f), surface.shininess);
    return ambient + diffuse + specular;
}

constexpr Color lighting_shadow(const Material& surface, const Light& light) noexcept {    
    const auto effective_color = surface.color * light.intensity;    
    const auto ambient = effective_color * surface.ambient;
    return ambient; //we're in shadow, so no diffuse or specular contribution needed.
}

