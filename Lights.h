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

constexpr Real schlick(const HitState& state) noexcept {
    //cosine of the angle between the eye and the normal vector
    auto cos = dot(state.eye_v, state.normal);
    //total internal reflection can only occur if n1 > n2
    if (state.n1 > state.n2) {
        const auto n_ratio = state.n1 / state.n2;
        const auto sin2_t = (n_ratio * n_ratio) * (1.0f - (cos * cos));
        if (sin2_t > 1.0f) {
            return 1.0f;
        }
        //compute cosine of theta_t using trig identity
        const auto cos_t = math::sqrt(1.0f - sin2_t);
        //when n1 > n2, use cos(theta_t) instead
        cos = cos_t;
    }
    const auto r0 = std::powf((state.n1 - state.n2) / (state.n1 + state.n2), 2);
    return r0 + (1.0f - r0) * std::powf(1.0f - cos, 5);
}
constexpr bool is_shadowed(const World& w, const Point& p) noexcept {
    const auto v = w.light.position - p;
    const auto distanceSq = magnitudeSq(v);
    const auto direction = normalize(v);
    const auto r = ray(p, direction); //ray from point towards light source
    try {
        const auto hit = closest(intersect(w, r)); //intersect allocates
        return (hit && (hit.t * hit.t) < distanceSq); //something is between us and the light.
    }
    catch (...) {}
    return false;
}

constexpr Color reflected_color(const World& w, const HitState& state, int remaining) noexcept;
constexpr Color refracted_color(const World& w, const HitState& state, int remaining) noexcept;

constexpr Color shade_hit(const World& w, const HitState& hit, int remaining = 4) noexcept {
    const auto shadowed = is_shadowed(w, hit.over_point);
    const auto surface_c = lighting(hit.surface(), hit.object(), w.light, hit.over_point, hit.eye_v, hit.normal, shadowed);
    const auto reflected_c = reflected_color(w, hit, remaining);
    const auto refracted_c = refracted_color(w, hit, remaining);
    if (hit.reflective() > 0 && hit.transparency() > 0) {
        const auto reflectance = schlick(hit);
        return surface_c + (reflected_c * reflectance) + (refracted_c * (1.0f - reflectance));
    }
    return surface_c + reflected_c + refracted_c;
}

constexpr Color color_at(const World& w, const Ray& r, int remaining = 4) noexcept {
    try {
        const auto xs = intersect(w, r); //allocates.
        const auto closestHit = closest(xs);
        if (closestHit) {
            const auto calcs = prepare_computations(closestHit, r, xs);
            return shade_hit(w, calcs, remaining);
        }
    }
    catch (...) {}
    return BLACK;
}

constexpr Color reflected_color(const World& w, const HitState& state, int remaining) noexcept {
    if (remaining <= 0 || state.reflective() == 0) {
        return BLACK;
    }
    const auto reflect_ray = ray(state.over_point, state.reflectv);
    const auto c = color_at(w, reflect_ray, remaining - 1);
    return c * state.reflective();
}

constexpr Color refracted_color(const World& w, const HitState& state, int remaining) noexcept {
    if (remaining <= 0 || state.transparency() == 0) {
        return BLACK;
    }
    const auto n_ratio = state.n1 / state.n2;
    const auto cos_i = dot(state.eye_v, state.normal);
    const auto sin2_t = math::square(n_ratio) * (1 - math::square(cos_i));
    if (sin2_t > 1.0f) {
        return BLACK; //total internal reflection
    }
    const auto cos_t = math::sqrt(1.0f - sin2_t);
    const auto direction = state.normal * (n_ratio * cos_i - cos_t) - state.eye_v * n_ratio;
    const auto refract_ray = ray(state.under_point, direction);
    return color_at(w, refract_ray, remaining - 1) * state.transparency();
}