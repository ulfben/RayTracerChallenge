#pragma once
#include "pch.h"
#include <numbers>
namespace Detail {
    Real constexpr sqrtNewtonRaphson(Real x, Real curr, Real prev) {
        return curr == prev ? curr
            : sqrtNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
    }
}

namespace math {
    constexpr auto PI = std::numbers::pi_v<Real>;
    constexpr auto TWO_PI = PI * 2.0f; //full circle
    constexpr auto HALF_PI = math::PI / 2.0f;
    constexpr auto INV_PI = std::numbers::inv_pi_v<Real>;
    constexpr auto TO_DEG = 180.0f / PI;
    constexpr auto TO_RAD = PI / 180.0f;

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

    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr T abs(T x) noexcept {
        return x < 0 ? -x : x; //in use only until std::abs becomes constexpr...
    }

    constexpr bool is_nan(Real x) noexcept {
        return x != x; //in use until std::is_nan becomes constexpr.
    }

    constexpr Real sqrt(Real x) noexcept {
        return x >= 0.0f && x < std::numeric_limits<Real>::infinity()
            ? Detail::sqrtNewtonRaphson(x, x, 0.0f)
            : std::numeric_limits<Real>::quiet_NaN();
    }

    static constexpr auto SHADOW_BIAS = 0.005f; //to avoid shadow acne due to spurious self-intersections
    static constexpr auto BOOK_EPSILON = 0.0001f; //1.0e-4f, value suggested as "good enough" by the book.
    static constexpr auto BRAZZY_EPSILON = 0.00001f; //1.0e-5f, https://github.com/brazzy/floating-point-gui.de    
    static constexpr auto GTEST_EPSILON = 0.000001f;// 1.0e-6f; //from Google Test
    static constexpr auto MACHINE_EPSILON = 0.000000119209f; //1.19209e-7f, == std::numeric_limits<Real>::epsilon();

    //from the book. fails on big, bigneg, infinities, oppoite, small, smallneg, ulp and zero
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr bool almost_equal(T a, T b, T epsilon) noexcept {
        return math::abs(a - b) <= epsilon;
    }

    //borrowed from QT    
    //doesn't handle infinities, ulp or near-zero values
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr bool qt_fuzzy_compare(T a, T b, [[maybe_unused]] T) noexcept {
        return (math::abs(a - b) * 100000.f <= std::min(std::abs(a), math::abs(b)));
    }

    //ported from Java implementation at https://floating-point-gui.de/errors/comparison/
    //  including test cases, see FloatCompareTests.h
    //  Currently failing all ulp tests. Does not work well with /fp:fast.
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr bool brazzy_nearly_equal(T a, T b, T epsilon) noexcept {
        if (a == b) { // shortcut, handles infinities
            return true;
        }
        const T absA = math::abs(a);
        const T absB = math::abs(b);
        const T diff = math::abs(a - b);
        if (a == 0 || b == 0 || (absA + absB < std::numeric_limits<T>::min())) {
            // a or b is zero or both are extremely close to it
            // relative error is less meaningful here
            //return diff < (epsilon * std::numeric_limits<T>::min()); //fails ulp-tests and my real-world data (close to zero)
            return diff < epsilon; //fails some Zero-tests but satisfies my real-world data.            
        }
        // use relative error
        return (diff / std::min((absA + absB), std::numeric_limits<T>::max())) < epsilon;
    }

    //borrowed from google test. Doesn't handle infinities, opposite, Small, SmallNeg or ulp
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr  bool gtest_almost_equals(T expected, T actual, T max_abs_error) noexcept {
        if (is_nan(expected) || is_nan(actual)) {
            return false;
        }
        const T abs_diff = std::abs(expected - actual);
        if (abs_diff <= max_abs_error) {
            return true;
        }
        const T abs_max = std::max(std::abs(expected), std::abs(actual));
        return abs_diff <= abs_max * max_abs_error;
    }

    //borrowed from Paul Floyd, https://accu.org/journals/overload/31/173/floyd/
    constexpr bool cmpEq(float a, float b, float epsilon = 1.0e-7f, float abstol =  1.0e-12f) noexcept{
        if (a == b) {
            return true;
        }
        const float diff = std::fabs(a - b);
        const float reltol = std::max(std::fabs(a), std::fabs(b)) * epsilon;
        return diff < reltol || diff < abstol;
    }

    //facade to let me swap the tested function and epsilon easily
    static constexpr bool float_cmp(Real a, Real b, Real epsilon) noexcept {
        return math::brazzy_nearly_equal(a, b, epsilon);
    }
    static constexpr bool float_cmp(Real a, Real b) noexcept {
        return float_cmp(a, b, math::BRAZZY_EPSILON);
    }
}