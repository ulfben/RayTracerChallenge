#pragma once
#include "pch.h"
#include "World.h"
#include "Ray.h"
#include "Shapes.h"

struct Intersection final {
    const Shapes* objPtr = nullptr;
    Real t{ 0 };

    explicit constexpr operator bool() const  {
        return objPtr != nullptr;
    }
    constexpr const Shapes& object() const  {
        assert(objPtr != nullptr);
        return *objPtr;
    }
    constexpr const Material& surface() const noexcept {
        return std::visit([](const auto& obj) -> const Material& { return obj.surface;  }, object());
    }
    constexpr bool operator<(const Intersection& that) const noexcept {
        return t < that.t;
    }  
    constexpr bool operator==(const Intersection& that) const noexcept {
        return object() == that.object() && math::float_cmp(t, that.t);
    }       
    constexpr auto operator<(Real time) const noexcept {
        return t < time;
    }
};

struct Intersections final {
    using size_type = uint8_t;
    using value_type = Intersection; 
    using container = std::vector<value_type>;
    using reference = container::reference;
    using const_reference = container::const_reference;
    using pointer = container::pointer;
    using const_pointer = container::const_pointer;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;
    container xs;

    explicit constexpr Intersections(std::initializer_list<value_type> intersections) noexcept : xs(intersections) {};

    constexpr const_reference operator[](size_type i) const noexcept {
        assert(i < size() && "Intersection::operator[i] index is out of bounds");
        return xs[i];
    }
    constexpr reference operator[](size_type i) noexcept {
        assert(i < size() && "Intersection::operator[i] index is out of bounds");
        return xs[i];
    }
    explicit constexpr operator bool() const noexcept {
        return !empty();
    }
    constexpr void push_back(value_type val) {
        xs.push_back(val);
    }
    constexpr void push_back(Intersections val) {
        xs.append_range(val);
    }

    constexpr void sort() noexcept { std::ranges::sort(xs, std::less<value_type>{}); }
    constexpr pointer data() noexcept { return xs.data(); }
    constexpr const_pointer data() const noexcept { return xs.data(); }
    constexpr size_type size() const noexcept { return narrow_cast<size_type>(xs.size()); }
    constexpr size_type count() const noexcept { return size(); }
    constexpr bool empty() const noexcept { return xs.empty(); }
    constexpr iterator begin() noexcept { return xs.begin(); }
    constexpr iterator end() noexcept { return xs.end(); }
    constexpr const_iterator begin() const noexcept { return xs.begin(); }
    constexpr const_iterator end() const noexcept { return xs.end(); }
    constexpr auto operator<=>(const Intersections& that) const noexcept = default;
};

constexpr auto intersection(Real t, const Shapes& obj) noexcept {
    return Intersection{ &obj, t };
}
constexpr auto intersections() noexcept {
    return Intersections{};
}
constexpr auto intersections(Intersection i1, Intersection i2) noexcept {
    return Intersections{ std::move(i1), std::move(i2) };
}
constexpr auto intersections(std::initializer_list<Intersection> is) noexcept {
    return Intersections(is);
}


constexpr std::pair<Real, Real> local_intersect([[maybe_unused]]const Plane& p, const Ray& local_ray)  {
    if (math::abs(local_ray.direction.y) < math::BOOK_EPSILON) {
        return {0.0f, 0.0f};
    }
    const auto t1 = -local_ray.origin.y / local_ray.direction.y;
    return {t1, t1};
};

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
constexpr std::pair<Real, Real> local_intersect(const Sphere& s, const Ray& local_ray)  {
    constexpr Real SPHERE_RADIUS = 1.0f; //assuming unit spheres for now    
    const Vector sphere_to_ray = local_ray.origin - s.position;
    const auto a = dot(local_ray.direction, local_ray.direction);
    const auto b = 2 * dot(local_ray.direction, sphere_to_ray);
    const auto col = dot(sphere_to_ray, sphere_to_ray) - SPHERE_RADIUS;
    const auto discriminant = (b * b) - (4.0f * a * col);
    if (discriminant < 0) {
        return {0.0f, 0.0f};
    }
    const auto sqrtOfDiscriminant = math::sqrt(discriminant);
    const auto t1 = (-b - sqrtOfDiscriminant) / (2 * a);
    const auto t2 = (-b + sqrtOfDiscriminant) / (2 * a);
    return {t1, t2};   
};

constexpr auto intersect(const Shapes& variant, const Ray& r)  {     
    const auto [t1, t2] = std::visit([&r](const auto& obj) {
        const auto local_ray = transform(r, inverse(obj.transform));
        return local_intersect(obj, local_ray);  }, variant 
    );
    if (t1 || t2) {
        return intersections({ intersection(t1, variant), intersection(t2, variant) });
    }
    return intersections();  
};

constexpr auto intersect(const World& world, const Ray& r) {
    Intersections result = intersections();     
    for (const auto& variant : world) {        
        result.push_back(intersect(variant, r));
    }    
    result.sort();
    return result;
};

//TODO: consider an alternative algorithm: remove + min_element
constexpr auto closest(const Intersections& xs) noexcept {    
    const auto iter = std::ranges::min_element(xs,
        // This comparison function allows us to find the smallest positive number 
        // by considering negative numbers as larger than positive numbers. 
        [](const Intersection& i1, const Intersection& i2) noexcept {
            if (i1.t < 0.0f) { return false; }
            if (i2.t < 0.0f) { return true; }
            return i1.t < i2.t;
        }
    );
    if (iter == std::end(xs) || *iter < 0.0f) {
        return Intersection{}; //empty set, or no positive T's in the set. Return 0. 
    }
    return *iter;
};

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

    constexpr HitState(const Intersection& closest, const Ray& r, const Intersections& xs) noexcept : HitState(closest, r){
        std::vector<const Shapes*> containers;
        for (const auto& i : xs) {
            const auto is_the_hit = i == closest;
            if (is_the_hit) {            
                n1 = containers.empty() ? 1.0f : ::surface(*containers.back()).refractive_index;            
            }
            
            if (const auto iter = std::ranges::find(containers, i.objPtr); iter != containers.end()) {
                containers.erase(iter); //this intersection must be exiting the object, remove it from the lists
            } else {
                containers.push_back(i.objPtr); //the intersection is entering the object, add it to the list
            }

            if (is_the_hit) {            
                n2 = containers.empty() ? 1.0f : ::surface(*containers.back()).refractive_index;                 
                break; //terminate the loop
            }
        }
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
    //coside of the nagle between the eye and the normal vector
    auto cos = dot(state.eye_v, state.normal);
    //total internal reflection can only occur if n1 > n2
    if (state.n1 > state.n2) {
        const auto n = state.n1 / state.n2;
        const auto sin2_t = (n * n) * (1.0f - (cos * cos));
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
    const auto surface_c = lighting(hit.surface(), w.light, hit.point, hit.eye_v, hit.normal, shadowed);
    const auto reflected_c = reflected_color(w, hit, remaining);
    const auto refracted_c = refracted_color(w, hit, remaining);
    const auto& mat = hit.surface();
    if (mat.reflective > 0 && mat.transparency > 0) {
        const auto reflectance = schlick(hit);
        return surface_c + (reflected_c * reflectance) + (refracted_c * (1.0f - reflectance));
    }
    return surface_c + reflected_c + refracted_c;
}

constexpr Color color_at(const World& w, const Ray& r, int remaining = 4) noexcept{    
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
    if (remaining <= 0 || state.surface().reflective == 0) {
        return BLACK;
    }
    const auto reflect_ray = ray(state.over_point, state.reflectv);
    const auto c = color_at(w, reflect_ray, remaining-1);
    return c * state.surface().reflective;
}

constexpr Color refracted_color(const World& w, const HitState& state, int remaining) noexcept {        
    if (remaining <= 0 || state.surface().transparency == 0) {
        return BLACK;
    }
    const auto n_ratio = state.n1 / state.n2;
    const auto cos_i = dot(state.eye_v, state.normal);
    const auto sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i));
    if (sin2_t > 1.0f) {
        return BLACK; //total internal reflection
    }
    const auto cos_t = math::sqrt(1.0f - sin2_t);
    const auto direction = state.normal * (n_ratio * cos_i - cos_t) - state.eye_v * n_ratio;
    const auto refract_ray = ray(state.under_point, direction);
    return color_at(w, refract_ray, remaining - 1) * state.surface().transparency;
}