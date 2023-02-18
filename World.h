#pragma once
#include "pch.h"
#include "Lights.h"
#include "Sphere.h"

struct World {
    static constexpr auto DEFAULT_WORLD_MATERIAL = material(color(0.8f, 1.0f, 0.6f), 0.9f, 0.7f, 0.2f);
    Light light = point_light(point(-10, 10, -10), color(1, 1, 1));
    std::vector<Sphere> objects{};
    
    constexpr World() noexcept {                
        auto s1 = sphere(DEFAULT_WORLD_MATERIAL); 
        auto s2 = sphere();
        s2.transform = scaling(0.5f, 0.5f, 0.5f);
        objects.push_back(s1); 
        objects.push_back(s2);
    }   

    constexpr bool contains(const Sphere& obj) const noexcept {        
        return std::ranges::find(objects, obj) != std::end(objects);
    }
};