#pragma once
#include "pch.h"
#include "Tuple.h"
struct Ray;
constexpr Point position(const Ray& r, Real time) noexcept;

struct Sphere {
    Point position; 
    Real radius;
    constexpr bool operator==(const Sphere& that) const noexcept {
        return position == that.position && math::almost_equal(radius, that.radius, math::BOOK_EPSILON);
    }
};

constexpr Sphere sphere(Point p, Real radius) noexcept {
    return Sphere{ p, radius };
}

template<class Object>
struct Intersection {            
    Object obj;
    Real t{0};

    constexpr bool operator==(const Intersection& that) const noexcept {
        return obj == that.obj && math::almost_equal(t, that.t, math::BOOK_EPSILON);
    }
};

template<class Object>
struct Intersections {
    using size_type = uint8_t;
    using value_type = Intersection<Object>;
    using reference = value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    value_type xs[2];
    uint8_t count{0};    
           
    constexpr value_type operator[](size_type i) const noexcept {
        assert(i < count && "Intersection::operator[i] index is out of bounds");
        return xs[i];
    }
    constexpr reference operator[](size_type i) noexcept {
        assert(i < count && "Intersection::operator[i] index is out of bounds");
        return xs[i];
    }
    constexpr pointer data() noexcept { return &xs[0]; }
    constexpr const_pointer data() const noexcept { return &xs[0]; }
    constexpr size_type size() const noexcept { return count; }
    constexpr iterator begin() noexcept { return data(); }
    constexpr iterator end() noexcept { return begin() + size(); }
    constexpr const_iterator begin() const noexcept { return data(); }
    constexpr const_iterator end() const noexcept { return begin() + size(); }  
    constexpr bool operator==(const Intersections& that) const noexcept {
        using std::ranges::equal;    
         return count == that.count && equal(*this, that,
            [](auto a, auto b) { return math::almost_equal(a, b, math::BOOK_EPSILON); });
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
    return Intersections{ std::move(i1), std::move(i2), 2 };
}

struct Ray {
    Point origin;
    Vector direction;

    constexpr Point position(Real time) const noexcept {    
        return ::position(*this, time);
    }
};

constexpr Ray ray(Point p, Vector dir) noexcept {
    assert(is_point(p) && is_vector(dir));
    return Ray{ p, dir };
}

constexpr Point position(const Ray& r, Real time) noexcept {    
    return r.origin + r.direction * time;
}

constexpr auto intersect(const Sphere& s, const Ray& r) noexcept {
    const Vector sphere_to_ray = r.origin - s.position;
    const auto a = dot(r.direction, r.direction);
    const auto b = 2 * dot(r.direction, sphere_to_ray);
    const auto c = dot(sphere_to_ray, sphere_to_ray) - 1;
    const auto discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) {
        return intersections<Sphere>();
    }
    const auto t1 = (-b - std::sqrt(discriminant)) / (2*a);
    const auto t2 = (-b + std::sqrt(discriminant)) / (2*a);
    return intersections( intersection(t1, s), intersection(t2, s));
};