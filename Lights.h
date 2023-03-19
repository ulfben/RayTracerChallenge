#pragma once
#include "pch.h"
#include "Tuple.h"

struct Light final {
    Point position;
    Color intensity;
};

constexpr Light point_light(Point p, Color i) noexcept {
    return Light{ p, i };
}