#pragma once
#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Shapes.h"

struct NullPattern final {
    Matrix4 transform{Matrix4Identity};
    constexpr Color at([[maybe_unused]] const Point& p) const noexcept { return BLACK; }
    explicit constexpr operator bool() const  { return false; }
    constexpr bool empty() const  { return true; }
    constexpr bool operator==(const NullPattern& that) const noexcept = default;
};

struct StripePattern final {
    Matrix4 transform{Matrix4Identity};
    Color a{};
    Color b{};
    constexpr Color at(const Point& p) const noexcept {
        return (math::int_floor(p.x) % 2 == 0) ? a : b;
    }
    explicit constexpr operator bool() const  { return true; }
    constexpr bool empty() const  { return false; }
    constexpr bool operator==(const StripePattern& that) const noexcept  = default;
    
};

using Patterns = std::variant<NullPattern, StripePattern>; 
template<typename T> 
concept patterns = std::is_same_v<NullPattern, T> || std::is_same_v<StripePattern, T>;

template<typename T>
requires patterns<T>
inline bool operator==(const T& t, const Patterns& v) {
    const T* c = std::get_if<T>(&v);
    return c && *c == t; // true if v contains a pattern that compares equal to v
}
template<typename T>
requires patterns<T>
inline bool operator==(const Patterns& v, const T& t) {
    return t == v;
};

constexpr Matrix4& transform(Patterns& variant) noexcept { 
    return std::visit([](auto& obj) noexcept -> Matrix4& { 
        return obj.transform;
    }, variant);    
};
constexpr const Matrix4& transform(const Patterns& variant) noexcept { 
    return std::visit([](const auto& obj) noexcept -> const Matrix4& { 
        return obj.transform;
    }, variant);    
};

constexpr auto null_pattern() noexcept {
    return NullPattern{};
};

constexpr auto stripe_pattern(Color a, Color b) noexcept {
    return StripePattern{ Matrix4Identity, a, b };
};

Color pattern_at(const Patterns& variant, const Point& p) noexcept {
    assert(!std::holds_alternative<NullPattern>(variant) && "pattern_at: called on NullPattern.");
    return std::visit([&p](const auto& obj) noexcept -> Color { return obj.at(p); }, variant);
};

Color pattern_at_obj(const Patterns& pattern, const Shapes& obj, const Point& world_point) noexcept {
    assert(!std::holds_alternative<NullPattern>(pattern) && "pattern_at: called on NullPattern.");
    const auto object_point = inverse(transform(obj)) * world_point;
    const auto pattern_point = inverse(transform(pattern)) * object_point;    
    return pattern_at(pattern, pattern_point);
};