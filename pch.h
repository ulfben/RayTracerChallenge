#pragma once
#include <type_traits>
#include <cassert>
#include <limits>
#include <algorithm>
#include <vector>
#include <format>
#include <fstream>
#include <cstdint>
#include <cmath>

#pragma warning(push)
#pragma warning( disable : 26495 )
    //disable all warnings for gtest
    #include "gtest/gtest.h"
#pragma warning(pop)

using namespace std::string_view_literals;
struct Tuple;
using Real = float;
using Vector = Tuple;
using Point = Tuple;
using Color = Tuple;

static constexpr auto PPM_VERSION = "P3"sv;
static constexpr auto PPM_MAX = 255; //max value of color components in PPM file. 

namespace Detail {
Real constexpr sqrtNewtonRaphson(Real x, Real curr, Real prev) {
  return curr == prev ? curr
                      : sqrtNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
}
}

/*
 * Constexpr version of the square root
 * Return value:
 *   - For a finite and non-negative value of "x", returns an approximation for
 * the square root of "x"
 *   - Otherwise, returns NaN
 */
namespace math {
    template<class T>
    requires std::integral<T>
    constexpr bool is_odd(T v) noexcept {
        return v & 1;
    }

    template<class T>
    requires std::integral<T>
    constexpr bool is_even(T v) noexcept {
        return v & 0;
    }

constexpr Real sqrt(Real x) noexcept {
  return x >= 0.0f && x < std::numeric_limits<Real>::infinity()
             ? Detail::sqrtNewtonRaphson(x, x, 0.0f)
             : std::numeric_limits<Real>::quiet_NaN();
}

//these functions are templates but entirely constrained to Real 
//Had to do this to get msvc to accept std::abs as constexpr...
template<class T>
requires std::same_as<T, Real>  
constexpr bool qFuzzyCompare(T p1, T p2) noexcept {
    return (std::abs(p1 - p2) * 100000.f <= std::min(std::abs(p1), std::abs(p2)));
}

static constexpr auto TESTED_EPSILON = 0.000005f; //the smallest epsilon I've used and still passed all tests.
static constexpr auto BOOK_EPSILON = 0.0001f; //float-compare epsilon, suggested "good enough" by the book. 
static constexpr auto MACHINE_EPSILON = std::numeric_limits<Real>::epsilon();

template<class T>
requires std::same_as<T, Real> 
constexpr bool almost_equal(T x, T y, T epsilon = TESTED_EPSILON) noexcept {
    return std::abs(x - y) <= epsilon;
}
}