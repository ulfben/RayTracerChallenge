#pragma once
#include "pch.h"
namespace Detail {
    Real constexpr sqrtNewtonRaphson(Real x, Real curr, Real prev) {
        return curr == prev ? curr
            : sqrtNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
    }
}

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

    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr T abs(T x) noexcept {
        return x < 0 ? -x : x; //in use only until std::abs becomes constexpr...
    }
     
    constexpr Real sqrt(Real x) noexcept {
        return x >= 0.0f && x < std::numeric_limits<Real>::infinity()
            ? Detail::sqrtNewtonRaphson(x, x, 0.0f)
            : std::numeric_limits<Real>::quiet_NaN();
    }

    static constexpr auto BOOK_EPSILON   = 0.0001f; //value suggested as "good enough" by the book.
    static constexpr auto BRAZZY_EPSILON = 0.00001f; //https://github.com/brazzy/floating-point-gui.de
    static constexpr auto TESTED_EPSILON = 0.000003f; //the smallest epsilon I've used with my function and still passed all tests.                                         
    static constexpr auto MACHINE_EPSILON= 0.000000119209f; //== std::numeric_limits<Real>::epsilon();

    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr bool almost_equal(T a, T b, T epsilon = TESTED_EPSILON) noexcept {
        return math::abs(a - b) <= epsilon;
    }

    //borrowed from QT
    //dummy argument added to match the other comparison interfaces (easier to facade during testing).
    constexpr bool fuzzy_compare(Real a, Real b, [[maybe_unused]] Real) noexcept {
        return (math::abs(a - b) * 100000.f <= std::min(std::abs(a), math::abs(b)));
    }

    //ported from Java implementation at https://floating-point-gui.de/errors/comparison/
    //  including test cases, see FloatCompareTests.h
    //  Currently failing all ulp tests. Does not work well with /fp:fast.
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr bool nearly_equal(T a, T b, T epsilon = BRAZZY_EPSILON) noexcept {
        if (a == b) { // shortcut, handles infinities
            return true;
        }
        const T absA = math::abs(a);
        const T absB = math::abs(b);
        const T diff = math::abs(a - b);
        if (a == 0 || b == 0 || (absA + absB < std::numeric_limits<T>::min())) {
            // a or b is zero or both are extremely close to it
            // relative error is less meaningful here
            return diff < (epsilon * std::numeric_limits<T>::min());
        }
        // use relative error
        return diff / std::min((absA + absB), std::numeric_limits<T>::max()) < epsilon;
    }
}