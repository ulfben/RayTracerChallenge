#pragma once
#pragma once
#include "pch.h"
#include "Tuple.h"

struct Pattern final {
    std::function<Color(const Point& p)> apply_pattern;

    Color pattern_at(const Point& p) const noexcept {
        return apply_pattern(p);
    }
};

constexpr auto strip_pattern(Color a, Color b) noexcept {
    const auto func = [a, b](const Point& p) noexcept {        
        return (math::floor_to_int(p.x) % 2 == 0) ? a : b;
    };
    return Pattern{func};
};