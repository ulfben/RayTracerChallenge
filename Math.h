#pragma once
#include "pch.h"
#include <numbers>
namespace Detail {
    template<class T>
        requires std::is_arithmetic_v<T>
    T constexpr sqrtNewtonRaphson(T x, T curr, T prev) {        
        return curr == prev ? curr
            : sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
    }
}

namespace math {
    constexpr auto PI = std::numbers::pi_v<Real>;
    constexpr auto INV_PI = std::numbers::inv_pi_v<Real>;
    constexpr auto TWO_PI = PI * 2.0f; //full circle
    constexpr auto HALF_PI = math::PI / 2.0f;
    constexpr auto TO_DEG = 180.0f / PI;
    constexpr auto TO_RAD = PI / 180.0f;
    constexpr auto MAX = std::numeric_limits<Real>::max();
    constexpr auto MIN = std::numeric_limits<Real>::lowest();  
    static constexpr auto SHADOW_BIAS = 0.005f; //to avoid shadow acne due to spurious self-intersections
    static constexpr auto BOOK_EPSILON = 0.0001f; //1.0e-4f, value suggested as "good enough" by the book.
    static constexpr auto BRAZZY_EPSILON = 0.00001f; //1.0e-5f, https://github.com/brazzy/floating-point-gui.de    
    static constexpr auto GTEST_EPSILON = 0.000001f;// 1.0e-6f; //from Google Test
    static constexpr auto MACHINE_EPSILON = 0.000000119209f; //1.19209e-7f, == std::numeric_limits<Real>::epsilon();


    template<class T>        
    constexpr T max(T a, T b, T c) noexcept {
        return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
    }
    
    template<class T>        
    constexpr T min(T a, T b, T c) noexcept {
       return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
    }

    template<class T>    
    constexpr bool is_between(T in, T min, T max) noexcept {
        return (in > min) && (in < max);
    }

    template<class T>        
    constexpr T lerp(T start, T end, T t) {        
       return (T(1.0) - t) * start + t * end;
    }

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
        if (std::is_constant_evaluated()) {
            return x == 0 ? 0 : (x < 0 ? -x : x);
        }
        return std::abs(x);
    }

    template<class T>        
    constexpr T is_zero(T v, T epsilon = BRAZZY_EPSILON) noexcept {
        return v == T(0) || math::abs(v) < epsilon;
    }


    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr bool is_nan(T x) noexcept {
        if (std::is_constant_evaluated()) {
            return x != x; //in use until std::is_nan becomes constexpr.
        }
        return std::isnan(x);
    }

    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr T sqrt(T x) noexcept {
        if (std::is_constant_evaluated()) {            
            return x >= T(0.0) && x < std::numeric_limits<T>::infinity()
                ? Detail::sqrtNewtonRaphson(x, x, T(0.0))
                : std::numeric_limits<T>::quiet_NaN();
        }
        return static_cast<T>(std::sqrt(x));
    }

    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr T square(T x) noexcept {
        return x * x;
    }
    
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr int int_ceil(T f) noexcept {
        if (std::is_constant_evaluated()) {
            const int i = static_cast<int>(f);
            return f > i ? i + 1 : i;
        }
        return static_cast<int>(std::ceil(f));

    }
    
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr int int_floor(T f) noexcept {
        if (std::is_constant_evaluated()) {
            const auto i = static_cast<int>(f);
            return f < i ? i - 1 : i;
        }
        return static_cast<int>(std::floor(f));
    }

    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr T floor(T f) noexcept {
        if (std::is_constant_evaluated()) {
            const auto i = static_cast<T>(f);
            return f < i ? i - 1 : i;
        }
        return static_cast<T>(std::floor(f));
    }



    //from the book. fails on big, bigneg, infinities, opposite, small, smallneg, ulp and zero
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
        return (math::abs(a - b) * 100000.f <= std::min(math::abs(a), math::abs(b)));
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
        const T abs_diff = math::abs(expected - actual);
        if (abs_diff <= max_abs_error) {
            return true;
        }
        const T abs_max = std::max(math::abs(expected), math::abs(actual));
        return abs_diff <= abs_max * max_abs_error;
    }

    //borrowed from Paul Floyd, https://accu.org/journals/overload/31/173/floyd/
    template<class T>
        requires std::is_arithmetic_v<T>
    constexpr bool cmpEq(T a, T b, T epsilon = 1.0e-7f, T abstol = 1.0e-12f) noexcept {
        if (a == b) {
            return true;
        }
        const auto diff = std::fabs(a - b);
        const auto reltol = std::max(std::fabs(a), std::fabs(b)) * epsilon;
        return diff < reltol || diff < abstol;
    }

    //facade to let me swap the tested function and epsilon easily
    template<class T>
        requires std::is_arithmetic_v<T>
    static constexpr bool float_cmp(T a, T b, T epsilon) noexcept {
        return math::brazzy_nearly_equal(a, b, epsilon);
    }

    template<class T>
       requires std::is_arithmetic_v<T>
    static constexpr bool float_cmp(T a, T b) noexcept {
        return float_cmp(a, b, math::BRAZZY_EPSILON);
    }
}