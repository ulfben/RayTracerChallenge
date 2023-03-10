#pragma once
#pragma once
#include "pch.h"
#include "Tuple.h"

struct Pattern final {
    std::function<Color(const Point& p)> apply_pattern;

    Color at(const Point& p) const noexcept {
        return apply_pattern(p);
    }
};

auto strip_pattern(Color a, Color b) noexcept {
    const auto func = [a, b](const Point& p) noexcept {        
        return (math::int_floor(p.x) % 2 == 0) ? a : b;
    };
    return Pattern{func};
};

Color pattern_at(const Pattern& pattern, const Point& p) noexcept {
    return pattern.at(p);
};