#pragma once
#include "pch.h"
#include "Tuple.h"
struct Ray;
constexpr Point position(const Ray& r, Real time) noexcept;

struct Sphere {
    Point position; 
    Real radius;
};

constexpr Sphere sphere(Point p, Real radius) noexcept {
    return Sphere{ p, radius };
}

struct Intersection {
    using size_type = uint8_t;
    using value_type = Real;
    using reference = value_type&;
    size_type count{0};
    value_type xs[2];
    
    constexpr value_type operator[](size_type i) const noexcept {
        assert(i < count && "Intersection::operator[i] index is out of bounds");
        return xs[i];
    }
    constexpr reference operator[](size_type i) noexcept {
        assert(i < count && "Intersection::operator[i] index is out of bounds");
        return xs[i];
    }
};

struct Intersections {
    using size_type = Intersection::size_type;
    using value_type = Intersection;
    using reference = value_type&;
    size_type count{0};
    value_type xs[2];
    
    constexpr value_type operator[](size_type i) const noexcept {
        assert(i < count && "Intersections::operator[i] index is out of bounds");
        return xs[i];
    }
    constexpr reference operator[](size_type i) noexcept {
        assert(i < count && "Intersections::operator[i] index is out of bounds");
        return xs[i];
    }
};

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

constexpr Intersection intersect(const Sphere& s, const Ray& r) noexcept {
    const Vector sphere_to_ray = r.origin - s.position;
    const auto a = dot(r.direction, r.direction);
    const auto b = 2 * dot(r.direction, sphere_to_ray);
    const auto c = dot(sphere_to_ray, sphere_to_ray) - 1;
    const auto discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) {
        return Intersection{};
    }
    const auto t1 = (-b - std::sqrt(discriminant)) / (2*a);
    const auto t2 = (-b + std::sqrt(discriminant)) / (2*a);
    return Intersection{ 2, t1, t2 };
};