#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Material.h"
#include "Pattern.h"

struct Light final {
    Point position;
    Color intensity;
};

constexpr Light point_light(Point p, Color i) noexcept {
    return Light{ p, i };
}

constexpr Color get_color_at(const Material& surface, const Point& p) noexcept {
    return has_pattern(surface) ? pattern_at(surface.pattern, p) : surface.color;
}

constexpr Color get_color_at(const Material& surface, const Shapes& obj, const Point& world_point) noexcept {    
    return has_pattern(surface) ? pattern_at(surface.pattern, obj, world_point) : surface.color;
}

constexpr Color lighting(const Color& color, const Material& surface, const Light& light, const Point& p, const Vector& eye, const Vector& normal, bool in_shadow = false) noexcept {
    const auto effective_color = color * light.intensity;
    if (in_shadow) {        
        return effective_color * surface.ambient; //we're in shadow, so no diffuse or specular contribution needed.        
    }    
    const auto direction_to_light = normalize(light.position - p);
    const auto ambient = effective_color * surface.ambient;
    const auto light_dot_normal = dot(direction_to_light, normal);
    if (light_dot_normal <= 0) {
        return ambient; //light dot normal, if negative, means the light is on the opposite side of the surface, so both the diffuse and specular component will be 0
    }
    const auto diffuse = effective_color * surface.diffuse * light_dot_normal;
    const auto reflection_v = reflect(-direction_to_light, normal);
    //reflection vector dot eye vector, if negative, means the light reflects away from the eye, so the specular component will be 0.
    const auto specular = light.intensity * surface.specular * std::pow(std::max(dot(reflection_v, eye), 0.0f), surface.shininess);
    return ambient + diffuse + specular;
}

constexpr Color lighting(const Material& surface, const Light& light, const Point& p, const Vector& eye, const Vector& normal, bool in_shadow = false) noexcept {
    const auto color = get_color_at(surface, p);
    return lighting(color, surface, light, p, eye, normal, in_shadow);
}

constexpr Color lighting(const Material& surface, const Shapes& obj, const Light& light, const Point& world_point, const Vector& eye, const Vector& normal, bool in_shadow = false) noexcept {
    const auto color = get_color_at(surface, obj, world_point);
    return lighting(color, surface, light, world_point, eye, normal, in_shadow);
}