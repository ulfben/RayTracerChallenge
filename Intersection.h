#pragma once
#include "pch.h"
#include "Ray.h"

template<class T>
struct Intersection final {
    const T* objPtr = nullptr;
    Real t{ 0 };

    explicit constexpr operator bool() const noexcept {
        return objPtr != nullptr;
    }
    constexpr const T& object() const noexcept {
        assert(objPtr != nullptr);
        return *objPtr;
    }
    constexpr const Material& surface() const noexcept {
        return object().surface;
    }
    constexpr bool operator==(const Intersection& that) const noexcept {
        return *objPtr == *that.objPtr && math::float_cmp(t, that.t);
    }
    constexpr bool operator==(const Real that) const noexcept {
        return math::float_cmp(t, that);
    }
    constexpr auto operator<=>(const Intersection& that) const noexcept {
        return t <=> that.t;
    }
    constexpr auto operator<=>(Real time) const noexcept {
        return t <=> time;
    }
};

template<class T>
struct Intersections final {
    using size_type = uint8_t;
    using value_type = Intersection<T>;
    using container = std::vector<value_type>;
    using reference = container::reference;
    using pointer = container::pointer;
    using const_pointer = container::const_pointer;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;
    container xs;

    explicit constexpr Intersections(std::initializer_list<value_type> intersections) noexcept : xs(intersections) {};

    constexpr value_type operator[](size_type i) const noexcept {
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

    constexpr void sort() {
        std::ranges::sort(xs, std::less<value_type>{});
    }

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

template<class T>
constexpr auto intersection(Real t, const T& obj) noexcept {
    return Intersection<T>{ &obj, t };
}
template<class T>
constexpr auto intersections() noexcept {
    return Intersections<T>{};
}
template<class InterSection>
constexpr auto intersections(InterSection i1, InterSection i2) noexcept {
    return Intersections{ std::move(i1), std::move(i2) };
}
template<class InterSection>
constexpr auto intersections(std::initializer_list<InterSection> is) noexcept {
    return Intersections(is);
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
constexpr auto intersect(const Sphere& s, const Ray& r) noexcept {
    constexpr Real SPHERE_RADIUS = 1.0f; //assuming unit spheres for now
    const auto ray2 = transform(r, inverse(s.transform));
    const Vector sphere_to_ray = ray2.origin - s.position;
    const auto a = dot(ray2.direction, ray2.direction);
    const auto b = 2 * dot(ray2.direction, sphere_to_ray);
    const auto col = dot(sphere_to_ray, sphere_to_ray) - SPHERE_RADIUS;
    const auto discriminant = (b * b) - (4.0f * a * col);
    if (discriminant < 0) {
        return intersections<Sphere>();
    }
    const auto sqrtOfDiscriminant = std::sqrt(discriminant);
    const auto t1 = (-b - sqrtOfDiscriminant) / (2 * a);
    const auto t2 = (-b + sqrtOfDiscriminant) / (2 * a);
    return intersections({ intersection(t1, s), intersection(t2, s) });
};

constexpr auto intersect(const World& world, const Ray& r) noexcept {
    Intersections result = intersections<World::value_type>(); //will have to be rethought once we have different shapes.
    for (const auto& obj : world) {
        result.push_back(intersect(obj, r));
    }
    result.sort();
    return result;
};

//TODO: limit template argument to Interactions-struct
//TODO: consider an alternative algorithm: remove + min_element
template <class Intersections>
constexpr auto closest(const Intersections& xs) noexcept {
    using value_type = Intersections::value_type;
    const auto iter = std::ranges::min_element(xs,
        // This comparison function allows us to find the smallest positive number 
        // by considering negative numbers as larger than positive numbers. 
        [](const value_type& i1, const value_type& i2) noexcept {
            if (i1.t < 0.0f) { return false; }
            if (i2.t < 0.0f) { return true; }
            return i1.t < i2.t;
        }
    );
    if (iter == std::end(xs) || *iter < 0.0f) {
        return value_type{}; //empty set, or no positive T's in the set. Return 0. 
    }
    return *iter;
};

template<class T>
struct HitState final { //"prepared computations", name to be figured out. 
    const T* objectPtr = nullptr; //the object we hit    
    Point point{}; //the point in world-space where the intersection occurs
    Point over_point{}; //slightly nudged point to avoid intersection precision errors causing "acne"
    Vector eye_v{}; //inverted, pointing back towards the camera
    Vector normal{}; //the normal of the point 
    Real t{ 0 }; //distance to hit
    bool inside = false;

    constexpr HitState(const Intersection<T>& i, const Ray& r) noexcept
        : objectPtr{ i.objPtr }, t{ i.t }, point{ position(r, i.t) }, eye_v{ -r.direction } {
        normal = normal_at(object(), point);
        if (dot(normal, eye_v) < 0.0f) {
            inside = true;
            normal = -normal;
        }
        //move the point VERY SLIGHTLY along the normal, to help with shading
        over_point = point + (normal * 0.005f);
    }

    explicit constexpr operator bool() const noexcept {
        return t != 0;
    }
    const Material& surface() const noexcept {
        return object().surface;
    }
    const T& object() const noexcept {
        assert(objectPtr && "HitState::object() called on empty HitState.");
        return *objectPtr;
    }
};

template<class T>
constexpr HitState<T> prepare_computations(const Intersection<T>& i, const Ray& r) noexcept {
    return HitState(i, r);
}

constexpr bool is_shadowed(const World& w, const Point& p){
    const auto v = w.light.position - p;
    const auto distanceSq = magnitudeSq(v);
    const auto direction = normalize(v);
    const auto r = ray(p, direction); //ray from point towards light source
    const auto hit = closest(intersect(w, r));
    return (hit && (hit.t*hit.t) < distanceSq); //something is between us and the light.
}

template<class T>
constexpr Color shade_hit(const World& w, const HitState<T>& hit) noexcept {
    if (is_shadowed(w, hit.over_point)) {
        return lighting_shadow(hit.surface(), w.light);
    }
    return lighting(hit.surface(), w.light, hit.over_point, hit.eye_v, hit.normal);
}

constexpr Color color_at(const World& w, const Ray& r) noexcept {
    const auto closestHit = closest(intersect(w, r));
    if (closestHit) {        
        return shade_hit(w, prepare_computations(closestHit, r));
    }
    return BLACK;
}

