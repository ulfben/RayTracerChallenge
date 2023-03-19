#pragma once
#include "pch.h"
#include "Ray.h"
#include "Shapes.h"
#include "Intersection.h"
struct HitState final {
    const Shapes* objectPtr = nullptr; //the object variant we hit    
    Point point{}; //the point in world-space where the intersection occurs
    Point over_point{}; //slightly nudged point to avoid intersection precision errors causing "acne"
    Point under_point{}; //sligthly nudged, for refraction and transparencies
    Vector eye_v{}; //inverted, pointing back towards the camera
    Vector normal{}; //the normal of the point 
    Vector reflectv{}; //reflection vector
    Real t{ 0 }; //distance to hit
    Real n1{ 1.0f }; //refractive index of the material we exited
    Real n2{ 1.0f }; //refractive index of the material we entered
    bool inside = false;

    constexpr HitState(const Intersection& i, const Ray& r) noexcept
        : objectPtr{ i.objPtr }, point{ position(r, i.t) }, eye_v{ -r.direction }, t{ i.t } {
        normal = normal_at(object(), point);
        if (dot(normal, eye_v) < 0.0f) {
            inside = true;
            normal = -normal;
        }
        //move the point out slightly along the normal to ensure that the shadow ray 
        // originates in front of the surface and not behind it (causing spurious self-shading)
        over_point = point + (normal * math::SHADOW_BIAS);
        under_point = point - (normal * math::SHADOW_BIAS);
        reflectv = reflect(r.direction, normal);
    }

    constexpr HitState(const Intersection& closest, const Ray& r, const Intersections& xs) : HitState(closest, r) {
        std::vector<const Shapes*> containers;
        containers.reserve(2);
        for (const auto& i : xs) {
            const auto is_the_hit = i == closest;
            if (is_the_hit) {
                n1 = containers.empty() ? 1.0f : ::surface(*containers.back()).refractive_index;
            }

            if (const auto iter = std::ranges::find(containers, i.objPtr); iter != containers.end()) {
                containers.erase(iter); //this intersection must be exiting the object, remove it from the lists
            }
            else {
                containers.push_back(i.objPtr); //the intersection is entering the object, add it to the list
            }

            if (is_the_hit) {
                n2 = containers.empty() ? 1.0f : ::surface(*containers.back()).refractive_index;
                break; //terminate the loop
            }
        }
    }

    constexpr Real reflective() const noexcept {
        return surface().reflective; //TODO: consider caching material properties in the hitstate object.
    }
    constexpr Real transparency() const noexcept {
        return surface().transparency;
    }

    explicit constexpr operator bool() const noexcept {
        return t != 0;
    }
    constexpr const Material& surface() const noexcept {
        return ::surface(object());
    }
    constexpr const Shapes& object() const noexcept {
        assert(objectPtr && "HitState::object() called on empty HitState.");
        return *objectPtr;
    }
};

constexpr HitState prepare_computations(const Intersection& i, const Ray& r) noexcept {
    return HitState(i, r);
}

constexpr HitState prepare_computations(const Intersection& i, const Ray& r, const Intersections& xs)  noexcept {
    return HitState(i, r, xs);
}