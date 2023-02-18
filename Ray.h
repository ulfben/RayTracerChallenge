#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Sphere.h"
#include "World.h"
struct Ray;
constexpr Point position(const Ray& r, Real time) noexcept;

template<class Object>
struct Intersection final {            
    Object obj;
    Real t{0};

    explicit constexpr operator bool() const noexcept {
        return t != 0;
    }
    constexpr bool operator==(const Intersection& that) const noexcept {
        return obj == that.obj && math::float_cmp(t, that.t);
    }
    constexpr bool operator==(const Real that) const noexcept {
        return math::float_cmp(t, that);
    }
    constexpr bool operator<(const Intersection& that) const noexcept {
        return this->t < that.t;
    }
     constexpr bool operator<(Real that) const noexcept {
        return t < that;
    }
};

template<class Object>
struct Intersections final {
    using size_type = uint8_t;
    using value_type = Intersection<Object>;
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
    constexpr size_type size() const noexcept { return static_cast<size_type>(xs.size()); }
    constexpr size_type count() const noexcept { return size(); }
    constexpr bool empty() const noexcept { return xs.empty(); }
    constexpr iterator begin() noexcept { return xs.begin(); }
    constexpr iterator end() noexcept { return xs.end(); }
    constexpr const_iterator begin() const noexcept { return xs.begin(); }
    constexpr const_iterator end() const noexcept { return xs.end(); }  
    constexpr bool operator==(const Intersections& that) const noexcept {
        using std::ranges::equal;    
         return size() == that.size() && equal(*this, that,
            [](auto a, auto b) noexcept { return math::float_cmp(a, b); });
    }
};

template<class Object>
constexpr auto intersection(Real t, Object obj) noexcept {
    return Intersection{std::move(obj), t};
}

template<class Object>
constexpr auto intersections() noexcept {
    return Intersections<Object>{};
}

template<class InterSection>
constexpr auto intersections(InterSection i1, InterSection i2) noexcept {
    return Intersections{ std::move(i1), std::move(i2) };
}
template<class InterSection>
constexpr auto intersections(std::initializer_list<InterSection> is) noexcept {
    return Intersections(is);
}

struct Ray final {
    Point origin;
    Vector direction;

    constexpr Point position(Real time) const noexcept {    
        return ::position(*this, time);
    }
    friend constexpr bool operator==(const Ray& lhs, const Ray& rhs) noexcept;
};
constexpr bool operator==(const Ray& lhs, const Ray& rhs) noexcept = default;

constexpr Ray operator*(const Matrix4& m, const Ray& r) noexcept {    
    return Ray{ m * r.origin, m * r.direction };
}
constexpr Ray operator*(const Ray& r, const Matrix4& m) noexcept {    
    return m * r;
}

constexpr Ray ray(Point p, Vector dir) noexcept {
    assert(is_point(p) && is_vector(dir));
    return Ray{ p, dir };
}

constexpr Point position(const Ray& r, Real time) noexcept {    
    return r.origin + r.direction * time;
}

constexpr Ray transform(const Ray& r, const Matrix4& m) noexcept {    
    return m * r;
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
    const auto t1 = (-b - sqrtOfDiscriminant) / (2*a);
    const auto t2 = (-b + sqrtOfDiscriminant) / (2*a);
    return intersections({ intersection(t1, s), intersection(t2, s) });
};

constexpr auto intersect(const World& world, const Ray& r) noexcept {
    Intersections result = intersections<World::value_type>();
    for (const auto& obj : world) {
        result.push_back(intersect(obj, r));
    }
    result.sort();
    return result;
};

//TODO: limit template argument to Interactions-struct
//TODO: consider an alternative algo: remove + min_element
template <class Intersections>
constexpr auto hit(const Intersections& xs) noexcept {
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