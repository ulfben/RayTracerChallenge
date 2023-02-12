#pragma once
#include "pch.h"
#include "Tuple.h"
struct Ray;
constexpr Point position(const Ray& r, Real time) noexcept;

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