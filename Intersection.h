#pragma once
#include "pch.h"
#include "Ray.h"
#include "Shapes.h"
#include "World.h"

struct Intersection final {
    const Shapes* objPtr = nullptr;
    Real t{ 0.0f };

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
    constexpr const Shapes& object_at(size_type i) const {
        assert(i < size() && "Intersection::operator[i] index is out of bounds");
        return xs[i].object();        
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
};
constexpr auto intersections() noexcept {
    return Intersections{};
};
constexpr auto intersections(size_t capacity) noexcept {
    return Intersections(capacity);
};
constexpr auto intersections(Intersection i1, Intersection i2) noexcept {
    return Intersections{ std::move(i1), std::move(i2) };
};
constexpr auto intersections(std::initializer_list<Intersection> is) noexcept {
    return Intersections(is);
};
constexpr auto intersections(std::span<Real> ts, const Shapes& shape) noexcept {
    auto xs = intersections(ts.size());
    for (auto t : ts) {
        xs.push_back(intersection(t, shape));
    }
    return xs;
};

constexpr auto intersect(const Shapes& variant, const Ray& r) {
    std::vector<Real> ts = std::visit([&r](const auto& obj) {
        const auto local_ray = transform(r, obj.inv_transform());
        return local_intersect(obj, local_ray);  }, variant
    );
    return intersections(ts, variant);
    /*if (t1 != T_MISS) {
        xs.push_back(intersection(t1, variant));
    }
    if (t2 != T_MISS) {
        xs.push_back(intersection(t2, variant));
    }
    return xs;*/
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
