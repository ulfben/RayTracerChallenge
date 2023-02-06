#pragma once
#include <type_traits>
#include <cassert>
#include <limits>
#include <algorithm>
#include <vector>
#include <format>
#include <fstream>
#include "cmath"

#pragma warning(push)
#pragma warning( disable : 26495 )
    //disable all warnings for gtest
    #include "gtest/gtest.h"
#pragma warning(pop)

using namespace std::string_view_literals;

static constexpr auto PPM_VERSION = "P3"sv;
static constexpr auto PPM_MAX = 255; //max value of color components in PPM file. 

namespace Detail {
float constexpr sqrtNewtonRaphson(float x, float curr, float prev) {
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

constexpr float sqrt(float x) noexcept {
  return x >= 0.0f && x < std::numeric_limits<float>::infinity()
             ? Detail::sqrtNewtonRaphson(x, x, 0.0f)
             : std::numeric_limits<float>::quiet_NaN();
}

template<class T>
requires std::floating_point<T>
constexpr bool almost_equal(T x, T y, int ulp){
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp
        // unless the result is subnormal
        || std::fabs(x-y) < std::numeric_limits<T>::min();
}

template<class T>
requires std::floating_point<T>
constexpr bool almost_equal(T x, T y) noexcept {
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon();
}
}