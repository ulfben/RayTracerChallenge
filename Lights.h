#pragma once
#include "pch.h"
#include "Tuple.h"

struct Light {    
    Point position;
    Color intensity;
};

constexpr Light point_light(Point p, Color i) noexcept {
    return Light{ p, i };
}

struct Material {
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