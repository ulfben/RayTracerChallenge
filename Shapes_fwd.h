#pragma once
#include "pch.h"
#include <variant>
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"

/*
 * About this header:
 *
 * We split the shape system into a small “forward” header to break a circular
 * dependency introduced by the design choice to model all shapes as concrete
 * value types inside a std::variant, while also allowing Group to behave as a
 * shape and to contain other shapes.
 *
 * In other words:
 *   - Group must be usable anywhere a Shape is.
 *   - Shapes must be able to contain Groups (for parent/child relationships).
 *
 * This creates a recursive type relationship that cannot be represented unless
 * we forward-declare Group here and only store it as a pointer inside the
 * variant.
 *
 * A cleaner architecture (and one I would use if starting over) is to store all
 * concrete shapes inside a World, and let Groups contain lightweight handles
 * (IDs) into that world. That avoids the need for forward declarations and keeps
 * ownership and hierarchy cleanly separated.
 */

struct Group;

using Shapes = std::variant<Sphere, Plane, Cube, Cylinder, Cone, Group*>;
template<typename T>
concept is_shape = std::is_same_v<Sphere, T> || std::is_same_v<Plane, T> || std::is_same_v<Cube, T> || std::is_same_v<Cylinder, T> || std::is_same_v<Cone, T>;

inline constexpr void set_parent(Shapes& variant, Group* g) noexcept;