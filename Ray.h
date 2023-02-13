#pragma once
#include "pch.h"
#include "Tuple.h"
struct Ray;
constexpr Point position(const Ray& r, Real time) noexcept;

struct Sphere {
    Point position; 
    Real radius;
    Matrix4 transform{ Matrix4Identity };

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

    explicit constexpr operator bool() const noexcept {
        return t != 0;
    }
    constexpr bool operator==(const Intersection& that) const noexcept {
        return obj == that.obj && math::almost_equal(t, that.t, math::BOOK_EPSILON);
    }
    constexpr bool operator==(const Real that) const noexcept {
        return math::almost_equal(t, that, math::BOOK_EPSILON);
    }
    constexpr bool operator<(const Intersection& that) const noexcept {
        return this->t < that.t;
    }
     constexpr bool operator<(Real that) const noexcept {
        return t < that;
    }
};

template<class Object>
struct Intersections {
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
    constexpr pointer data() noexcept { return xs.data(); }
    constexpr const_pointer data() const noexcept { return xs.data(); }
    constexpr size_type size() const noexcept { return static_cast<size_type>(xs.size()); }
    constexpr bool empty() const noexcept { return xs.empty(); }
    constexpr iterator begin() noexcept { return xs.begin(); }
    constexpr iterator end() noexcept { return xs.end(); }
    constexpr const_iterator begin() const noexcept { return xs.begin(); }
    constexpr const_iterator end() const noexcept { return xs.end(); }  
    constexpr bool operator==(const Intersections& that) const noexcept {
        using std::ranges::equal;    
         return size() == that.size() && equal(*this, that,
            [](auto a, auto b) noexcept { return math::almost_equal(a, b, math::BOOK_EPSILON); });
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

constexpr bool operator==(const Ray& lhs, const Ray& rhs) noexcept {    
    return lhs.origin == rhs.origin && lhs.direction == rhs.direction;
}

constexpr Ray operator*(const Matrix4& m, const Ray& r) noexcept {    
    return Ray{ m * r.origin, m * r.direction };
}
constexpr Ray operator*(const Ray& r, const Matrix4& m) noexcept {    
    return m * r;
}

constexpr Ray transform(const Ray& r, Matrix4 m) noexcept {    
    return m * r;
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
constexpr auto intersect(const Sphere& s, const Ray& r) noexcept {
    constexpr auto SPHERE_RADIUS = 1; //assuming unit spheres for now
    const Vector sphere_to_ray = r.origin - s.position;
    const auto a = dot(r.direction, r.direction);
    const auto b = 2 * dot(r.direction, sphere_to_ray);
    const auto c = dot(sphere_to_ray, sphere_to_ray) - SPHERE_RADIUS;
    const auto discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) {
        return intersections<Sphere>();
    }
    const auto sqrtOfDiscriminant = std::sqrt(discriminant);
    const auto t1 = (-b - sqrtOfDiscriminant) / (2*a);
    const auto t2 = (-b + sqrtOfDiscriminant) / (2*a);
    return intersections({ intersection(t1, s), intersection(t2, s) });
};

//TODO: limit template argument to Interactions-struct
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