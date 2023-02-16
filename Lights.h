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
    Color col = color(1, 1, 1);
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

constexpr Color lighting(const Material& surface, const Light& light, const Point& p, const Vector& eye, const Vector& normal) noexcept {    
    const auto effective_color = surface.col * light.intensity;
    const auto light_v = normalize(light.position - p); //find the direction to the light source    
    const Color ambient = effective_color * surface.ambient;
    Color diffuse; //black by default
    Color specular;     
    //light_dot_normal is the cosine of the angle between the light vector and the normal vector. 
    //a negative number means the light is on the opposite side of the surface.
    if (const auto light_dot_normal = dot(light_v, normal); light_dot_normal >= 0) {
        diffuse = effective_color * surface.diffuse * light_dot_normal; 
        //reflect_dot_eye is the cosine of the angle between reflection vector and the eye vector. 
        //a negative number means the light reflects away from the eye.
        const auto reflectv = reflect(-light_v, normal);
        const auto reflect_dot_eye = dot(reflectv, eye);
        if (reflect_dot_eye >= 0) { //compute specular
            const auto factor = std::pow(reflect_dot_eye, surface.shininess);
            specular = light.intensity * surface.specular * factor;
        } /*else: specular is black*/
    } /*else: diffuse and specular are both black*/
    return ambient + diffuse + specular;
}