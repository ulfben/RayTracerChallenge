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
    friend constexpr bool operator==(const Material& lhs, const Material& rhs) noexcept;
};
constexpr bool operator==(const Material& lhs, const Material& rhs) noexcept = default;


constexpr Material material() noexcept {
    return {};
}
constexpr Material material(Color c, Real ambient = 0.9f, Real diffuse = 0.9f, Real specular = 0.9f, Real shininess = 200.0f) noexcept {
    return Material{c, ambient, diffuse, specular, shininess};    
}

constexpr Color lighting(const Material& surface, const Light& light, const Point& p, const Vector& eye, const Vector& normal) noexcept {    
    const auto effective_color = surface.color * light.intensity;
    const auto direction_to_light = normalize(light.position - p);
    const auto ambient = effective_color * surface.ambient;    
    const auto light_dot_normal = dot(direction_to_light, normal);
    if (light_dot_normal <= 0) {
        return ambient; //light dot normal, if negative, means the light is on the opposide side of the surface, so both the diffuse and specular component will be 0
    }
    const auto diffuse = effective_color * surface.diffuse * light_dot_normal; 
    const auto reflection_v = reflect(-direction_to_light, normal);
    //reflection vector dot eye vector, if negative, means the light reflects away from the eye, so the specular component will be 0.
    const auto specular = light.intensity * surface.specular * std::pow(std::max(dot(reflection_v, eye), 0.0f), surface.shininess); 
    return ambient + diffuse + specular;
}

