#pragma once
#include "pch.h"
#include "World.h"
#include "Ray.h"
#include "Shapes.h"

struct Intersection final {
    const Shapes* objPtr = nullptr;
    Real t{ 0 };

    explicit constexpr operator bool() const {
        return objPtr != nullptr;
    }
    constexpr const Shapes& object() const {
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

    explicit constexpr Intersections(size_t capacity) {
        xs.reserve(capacity);
    };
    explicit constexpr Intersections(std::initializer_list<value_type> intersections) : xs(intersections) {};

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
constexpr auto intersections(size_t capacity) noexcept {
    return Intersections(capacity);
}
constexpr auto intersections(Intersection i1, Intersection i2) noexcept {
    return Intersections{ std::move(i1), std::move(i2) };
}
constexpr auto intersections(std::initializer_list<Intersection> is) noexcept {
    return Intersections(is);
}

constexpr std::pair<Real, Real> local_intersect([[maybe_unused]] const Cylinder& cylinder, Ray local_ray) noexcept {
    using math::square, math::sqrt, math::is_between;
    const auto a = square(local_ray.direction.x) + square(local_ray.direction.z);
    if (a < math::BOOK_EPSILON) { //close to 0
        return { 0.0f, 0.0f }; //ray is ~parallel to the Y axis so we can't collide.
    }
    const auto b = 2.0f * local_ray.origin.x * local_ray.direction.x +
                   2.0f * local_ray.origin.z * local_ray.direction.z;
    const auto c = square(local_ray.origin.x) + square(local_ray.origin.z) - 1.0f;
    const auto discriminant = square(b) - 4.0f * a * c;
    if (discriminant < 0.0f) {
        return { 0.0f, 0.0f }; //ray does not intersect with the cylinder
    }
    auto t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    auto t2 = (-b + sqrt(discriminant)) / (2.0f * a);
    //if (t1 > t2) {
    //    std::swap(t1, t2);
    //}
    std::pair result = { t1, t2 };
    if (is_bounded(cylinder)) { //let's compute the height of each intersection
        const auto y1 = local_ray.origin.y + t1 * local_ray.direction.y;
        if (!is_between(y1, cylinder.minimum, cylinder.maximum)) {
            result.first = 0.0f; //the first intersection happened above or below the cylinder limits.
        }
        const auto y2 = local_ray.origin.y + t2 * local_ray.direction.y;
        if (!is_between(y2, cylinder.minimum, cylinder.maximum)) {
            result.second = 0.0f; //the second intersection happened above or below the cylinder limits.
        }        
    }
    return result;
}

constexpr std::pair<Real, Real> local_intersect([[maybe_unused]] const Cube& cube, const Ray& local_ray) noexcept {
    const auto [xtmin, xtmax] = check_axis(local_ray.origin.x, local_ray.direction.x);
    const auto [ytmin, ytmax] = check_axis(local_ray.origin.y, local_ray.direction.y);
    const auto [ztmin, ztmax] = check_axis(local_ray.origin.z, local_ray.direction.z);
    const auto tmin = math::max(xtmin, ytmin, ztmin);
    const auto tmax = math::min(xtmax, ytmax, ztmax);
    if (tmin > tmax) return { 0.0f, 0.0f };
    return { tmin, tmax };
}

constexpr std::pair<Real, Real> local_intersect([[maybe_unused]] const Plane& p, const Ray& local_ray) {
    if (math::abs(local_ray.direction.y) < math::BOOK_EPSILON) {
        return { 0.0f, 0.0f };
    }
    const auto t1 = -local_ray.origin.y / local_ray.direction.y;
    return { t1, t1 };
};

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
constexpr std::pair<Real, Real> local_intersect([[maybe_unused]] const Sphere& s, const Ray& local_ray) {
    constexpr Real SPHERE_RADIUS = 1.0f; //assuming unit spheres for now    
    const Vector sphere_to_ray = local_ray.origin;/* -s.position; sphere is always located at 0,0,0*/
    const auto a = dot(local_ray.direction, local_ray.direction);
    const auto b = 2 * dot(local_ray.direction, sphere_to_ray);
    const auto col = dot(sphere_to_ray, sphere_to_ray) - SPHERE_RADIUS;
    const auto discriminant = (b * b) - (4.0f * a * col);
    if (discriminant < 0) {
        return { 0.0f, 0.0f };
    }
    const auto sqrtOfDiscriminant = math::sqrt(discriminant);
    const auto t1 = (-b - sqrtOfDiscriminant) / (2 * a);
    const auto t2 = (-b + sqrtOfDiscriminant) / (2 * a);
    return { t1, t2 };
};

constexpr auto intersect(const Shapes& variant, const Ray& r) {
    const auto [t1, t2] = std::visit([&r](const auto& obj) {
        const auto local_ray = transform(r, obj.inv_transform());
        return local_intersect(obj, local_ray);  }, variant
    );
    auto xs = intersections(2);
    if (t1 != 0.0f) {
        xs.push_back(intersection(t1, variant));
    }
    if (t2 != 0.0f) {
        xs.push_back(intersection(t2, variant));
    }
    return xs;
};

constexpr auto intersect(const World& world, const Ray& r) {
    Intersections result = intersections(world.size() * 2);//reserve space to store 2 intersections per object in the world
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

