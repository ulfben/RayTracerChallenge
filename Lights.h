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

constexpr Material material() noexcept {
    return {};
}
constexpr Material material(Color c, Real ambient = 0.1f, Real diffuse = 0.9f, Real specular = 0.9f, Real shininess = 200.0f) noexcept {
    return Material{c, ambient, diffuse, specular, shininess};    
}

constexpr Color lighting(const Material& surface, const Light& light, const Point& p, const Vector& eye, const Vector& normal) noexcept {    
    const auto effective_color = surface.color * light.intensity;
    const auto direction_to_light = normalize(light.position - p);
    const Color ambient = effective_color * surface.ambient;
    Color diffuse{}; //black by default
    Color specular{};
    //light dot normal, if negative, means the light is on the opposide side of the surface, so the diffuse and specular component will be 0
    if (const auto light_dot_normal = dot(direction_to_light, normal); light_dot_normal >= 0) {
        diffuse = effective_color * surface.diffuse * light_dot_normal;
        //reflection dot eye, if negative, means the light reflects away from the eye, so the specular component will be 0.
        const auto reflection_v = reflect(-direction_to_light, normal);
        if (const auto reflect_dot_eye = dot(reflection_v, eye); reflect_dot_eye > 0.0f) {
            const auto factor = std::pow(reflect_dot_eye, surface.shininess);
            specular = light.intensity * surface.specular * factor;
        } /*else: specular is 0*/

    } /*else: diffuse and specular are both 0*/
    return ambient + diffuse + specular;
}

