#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"

struct NullPattern final {
    constexpr Color at([[maybe_unused]] const Point& p) const noexcept { return BLACK; }
    explicit constexpr operator bool() const noexcept { return false; }    
    constexpr bool operator==(const NullPattern& that) const noexcept = default;
    constexpr const Matrix4& transform() const noexcept { return _transform; }    
    constexpr const Matrix4& inv_transform() const noexcept { return _invTransform; }
    constexpr void setTransform([[maybe_unused]]Matrix4 mat) noexcept {}
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
};

struct TestPattern final {
    constexpr Color at([[maybe_unused]] const Point& p) const noexcept { return color(p.x, p.y, p.z); }
    explicit constexpr operator bool() const noexcept { return true; }    
    constexpr bool operator==(const TestPattern& that) const noexcept = default;
    constexpr const Matrix4& transform() const noexcept { return _transform; }    
    constexpr const Matrix4& inv_transform() const noexcept { return _invTransform; }
    constexpr void setTransform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
};

struct StripePattern final {
    constexpr StripePattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{b_} {
        setTransform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {
        return (math::int_floor(p.x) % 2 == 0) ? a : b;
    }
    constexpr void setTransform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& transform() const noexcept {
        return _transform;
    }    
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }    
    explicit constexpr operator bool() const noexcept { return true; }    
    constexpr bool operator==(const StripePattern& that) const noexcept  = default; 
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};

struct GradientPattern final {
    constexpr GradientPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{b_} {
        setTransform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {              
       const auto dx = p.x - math::floor(p.x);
       return lerp(a, b,  dx);       
    }
    constexpr void setTransform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& transform() const noexcept {
        return _transform;
    }    
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }    
    constexpr bool operator==(const GradientPattern& that) const noexcept  = default;
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};
struct RadialGradientPattern final {
    constexpr RadialGradientPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{b_} {
        setTransform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {              
        const auto dist_from_center = magnitude(vector(p.x, p.y, p.z));
        const auto fraction = dist_from_center - math::floor(dist_from_center);
       return lerp(a, b, fraction);       
    }
    constexpr void setTransform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& transform() const noexcept {
        return _transform;
    }    
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }    
    constexpr bool operator==(const RadialGradientPattern& that) const noexcept  = default;
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};
struct RingPattern final {
    constexpr RingPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{b_} {
        setTransform(std::move(mat));
    }
    constexpr void setTransform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr Color at(const Point& p) const noexcept {         
        const auto distance_from_center = math::sqrt((p.x * p.x) + (p.z * p.z));
        const auto mod = math::int_floor(distance_from_center) % 2;        
        return mod == 0 ? a : b; 
    }   
    constexpr const Matrix4& transform() const noexcept {
        return _transform;
    }    
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }    
    constexpr bool operator==(const RingPattern& that) const noexcept  = default;    
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};
struct CheckersPattern final {    
    constexpr CheckersPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{b_} {
        setTransform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {        
        const auto val = static_cast<int>(math::floor(p.x) + math::floor(p.y) + math::floor(p.z));
        return (val % 2 == 0) ? a : b; 
    }
    constexpr void setTransform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& transform() const noexcept {
        return _transform;
    }    
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }    
    constexpr bool operator==(const CheckersPattern& that) const noexcept  = default;    
private: 
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};

constexpr auto null_pattern() noexcept {
    return NullPattern{};
};
constexpr auto test_pattern() noexcept {
    return TestPattern();
};

constexpr auto stripe_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return StripePattern( std::move(m), a, b );
};
constexpr auto gradient_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return GradientPattern( std::move(m), a, b );
};
constexpr auto radial_gradient_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return RadialGradientPattern( std::move(m), a, b );
};
constexpr auto ring_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return RingPattern( std::move(m), a, b );
};
constexpr auto checkers_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return CheckersPattern( std::move(m), a, b );
};

using Patterns = std::variant<NullPattern, TestPattern, StripePattern, GradientPattern, RingPattern, CheckersPattern, RadialGradientPattern>; 
template<typename T> 
concept is_pattern = std::is_same_v<NullPattern, T> || std::is_same_v<TestPattern, T> || 
                    std::is_same_v<StripePattern, T> || std::is_same_v<GradientPattern, T> || 
                    std::is_same_v<RingPattern, T> || std::is_same_v<CheckersPattern, T> || 
                    std::is_same_v<RadialGradientPattern, T>;

template<typename T>
requires is_pattern<T>
inline bool operator==(const T& t, const Patterns& v) {
    const T* c = std::get_if<T>(&v);
    return c && *c == t; // true if v contains a pattern that compares equal to v
}

inline bool operator==(const Patterns& v, const is_pattern auto& t) {
    return t == v;
};

constexpr const Matrix4& transform(const Patterns& variant) noexcept { 
    return std::visit([](const auto& pattern) noexcept -> const Matrix4& { 
        return pattern.transform();
    }, variant);    
};
constexpr const Matrix4& invTransform(const Patterns& variant) noexcept { 
    return std::visit([](const auto& pattern) noexcept -> const Matrix4& { 
        return pattern.inv_transform();
    }, variant);    
};

Color pattern_at(const Patterns& variant, const Point& p) noexcept {
    assert(!std::holds_alternative<NullPattern>(variant) && "pattern_at: called on NullPattern.");
    return std::visit([&p](const auto& pattern) noexcept -> Color { return pattern.at(p); }, variant);
};

//#include "Shapes.h"
//template<typename T>
//requires is_shape<T>
Color pattern_at(const Patterns& pattern, const auto& obj, const Point& world_point) noexcept {
    assert(!std::holds_alternative<NullPattern>(pattern) && "pattern_at: called on NullPattern.");
    const auto object_point = invTransform(obj) * world_point;
    const auto pattern_point = invTransform(pattern) * object_point;        
    return pattern_at(pattern, pattern_point);
};