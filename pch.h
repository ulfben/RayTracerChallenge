//
// pch.h
//

#pragma once
#include "gtest/gtest.h"
#include <cassert>
#include <limits>

namespace Detail {
float constexpr sqrtNewtonRaphson(float x, float curr, float prev) {
  return curr == prev ? curr
                      : sqrtNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
}
} // namespace Detail

/*
 * Constexpr version of the square root
 * Return value:
 *   - For a finite and non-negative value of "x", returns an approximation for
 * the square root of "x"
 *   - Otherwise, returns NaN
 */
namespace math {
float constexpr sqrt(float x) noexcept {
  return x >= 0.0f && x < std::numeric_limits<float>::infinity()
             ? Detail::sqrtNewtonRaphson(x, x, 0.0f)
             : std::numeric_limits<float>::quiet_NaN();
}
} // namespace math