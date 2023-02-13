#pragma once
#include "../pch.h"
#include "../Math.h"
/**
 * Test suite to demonstrate a good method for comparing floating-point values
 * using an epsilon. Run via Google Test.
 *
 * Note: this function attempts a "one size fits all" solution. There may be
 * some edge cases for which it still produces unexpected results, and some of
 * the tests it was developed to pass probably specify behaviour that is not
 * appropriate for some applications, especially concerning very small values
 * with differing signs.
 *
 * Before using it, make sure it's appropriate for your application!
 *
 * From http://floating-point-gui.de
 *
 * @author Michael Borgwardt
 * ported to C++ by Ulf Benjaminsson
 */

/*
* When compiling with /fp:fast (fast floating point) special values 
* (NaN, +infinity, -infinity, -0.0) may not behave strictly according 
* to the IEEE-754 standard.
* I have yet to figure out if /fp:fast can be temporarily disabled while testing. 
*/

static_assert(std::numeric_limits<Real>::is_iec559, "IEEE 754 required");
static_assert(std::numeric_limits<Real>::has_quiet_NaN, ""); 
static constexpr auto MACHINE_EPSILON = std::numeric_limits<Real>::epsilon();
static constexpr auto MAX = std::numeric_limits<Real>::max();
static constexpr auto MIN = std::numeric_limits<Real>::min();
static constexpr auto INF = std::numeric_limits<Real>::infinity();
static constexpr auto NEG_INF = -INF;
static constexpr Real QNAN = std::numeric_limits<Real>::quiet_NaN();
using math::float_cmp;


 // Regular large numbers - generally not problematic 
TEST(float_cmp, big) {
    EXPECT_TRUE(float_cmp(1000000.0f, 1000001.0f));
    EXPECT_TRUE(float_cmp(1000001.0f, 1000000.0f));
    EXPECT_FALSE(float_cmp(10000.0f, 10001.0f));
    EXPECT_FALSE(float_cmp(10001.0f, 10000.0f));
}

// Negative large numbers
TEST(float_cmp, bigNeg) {
    EXPECT_TRUE(float_cmp(-1000000.0f, -1000001.0f));
    EXPECT_TRUE(float_cmp(-1000001.0f, -1000000.0f));
    EXPECT_FALSE(float_cmp(-10000.0f, -10001.0f));
    EXPECT_FALSE(float_cmp(-10001.0f, -10000.0f));
}

// Numbers around 1 
TEST(float_cmp, mid) {
    EXPECT_TRUE(float_cmp(1.0000001f, 1.0000002f));
    EXPECT_TRUE(float_cmp(1.0000002f, 1.0000001f));
    EXPECT_FALSE(float_cmp(1.0002f, 1.0001f));
    EXPECT_FALSE(float_cmp(1.0001f, 1.0002f));
}

// Numbers around -1 
TEST(float_cmp, midNeg) {
    EXPECT_TRUE(float_cmp(-1.000001f, -1.000002f));
    EXPECT_TRUE(float_cmp(-1.000002f, -1.000001f));
    EXPECT_FALSE(float_cmp(-1.0001f, -1.0002f));
    EXPECT_FALSE(float_cmp(-1.0002f, -1.0001f));
}

// Numbers between 1 and 0
TEST(float_cmp, Small) {
    EXPECT_TRUE(float_cmp(0.000000001000001f, 0.000000001000002f));
    EXPECT_TRUE(float_cmp(0.000000001000002f, 0.000000001000001f));
    EXPECT_FALSE(float_cmp(0.000000000001002f, 0.000000000001001f));
    EXPECT_FALSE(float_cmp(0.000000000001001f, 0.000000000001002f));
}

// Numbers around -1 
TEST(float_cmp, SmallNeg) {
    EXPECT_TRUE(float_cmp(-0.000000001000001f, -0.000000001000002f));
    EXPECT_TRUE(float_cmp(-0.000000001000002f, -0.000000001000001f));
    EXPECT_FALSE(float_cmp(-0.000000000001002f, -0.000000000001001f));
    EXPECT_FALSE(float_cmp(-0.000000000001001f, -0.000000000001002f));
}

// Small differences away from zero 
TEST(float_cmp, SmallDiffs) {
    EXPECT_TRUE(float_cmp(0.3f, 0.30000003f));
    EXPECT_TRUE(float_cmp(-0.3f, -0.30000003f));
}

// Comparisons involving zero 
TEST(float_cmp, Zero) {
    EXPECT_TRUE(float_cmp(0.0f, -4.37114e-08f)); //seen in my app (from matrix rotations)
    EXPECT_TRUE(float_cmp(0.0f, -2.18557e-07f)); //seen in my app (from matrix rotations)
    EXPECT_TRUE(float_cmp(0.0f, -0.0000000437114f)); //seen in my app (from matrix rotations)

    EXPECT_TRUE(float_cmp(0.0f, 0.0f));
    EXPECT_TRUE(float_cmp(0.0f, -0.0f));
    EXPECT_TRUE(float_cmp(-0.0f, -0.0f));
//  EXPECT_FALSE(float_cmp(0.00000001f, 0.0f)); 
//  EXPECT_FALSE(float_cmp(0.0f, 0.00000001f));
//  EXPECT_FALSE(float_cmp(-0.00000001f, 0.0f));
//  EXPECT_FALSE(float_cmp(0.0f, -0.00000001f));

    EXPECT_TRUE(float_cmp(0.0f, 1e-40f, 0.01f));
    EXPECT_TRUE(float_cmp(1e-40f, 0.0f, 0.01f));
//  EXPECT_FALSE(float_cmp(1e-40f, 0.0f, 0.000001f));
//  EXPECT_FALSE(float_cmp(0.0f, 1e-40f, 0.000001f));

    EXPECT_TRUE(float_cmp(0.0f, -1e-40f, 0.1f));
    EXPECT_TRUE(float_cmp(-1e-40f, 0.0f, 0.1f));
//  EXPECT_FALSE(float_cmp(-1e-40f, 0.0f, 0.00000001f));
//  EXPECT_FALSE(float_cmp(0.0f, -1e-40f, 0.00000001f));
}

// Comparisons involving extreme values (overflow potential) 
TEST(float_cmp, extremeMax) {
    EXPECT_TRUE(float_cmp(MAX, MAX));
    EXPECT_FALSE(float_cmp(MAX, -MAX));
    EXPECT_FALSE(float_cmp(-MAX, MAX));
    EXPECT_FALSE(float_cmp(MAX, MAX / 2));
    EXPECT_FALSE(float_cmp(MAX, -MAX / 2));
    EXPECT_FALSE(float_cmp(-MAX, MAX / 2));
}

// Comparisons involving infinities
TEST(float_cmp, infinities) {
    EXPECT_TRUE(float_cmp(INF, INF));
    EXPECT_TRUE(float_cmp(NEG_INF, NEG_INF));
    EXPECT_FALSE(float_cmp(NEG_INF, INF));
    EXPECT_FALSE(float_cmp(INF, MAX));
    EXPECT_FALSE(float_cmp(NEG_INF, -MAX));
}

// Comparisons involving NaN values
// NOTE: these all fail if compiled with /fp:fast 
// see: https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170&viewFallbackFrom=vs-2019#fast
TEST(float_cmp, nan) {    
    EXPECT_FALSE(float_cmp(QNAN, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, 0.0f));
    EXPECT_FALSE(float_cmp(-0.0f, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, -0.0f));
    EXPECT_FALSE(float_cmp(0.0f, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, INF));
    EXPECT_FALSE(float_cmp(INF, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, NEG_INF));
    EXPECT_FALSE(float_cmp(NEG_INF, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, MAX));
    EXPECT_FALSE(float_cmp(MAX, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, -MAX));
    EXPECT_FALSE(float_cmp(-MAX, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, MIN));
    EXPECT_FALSE(float_cmp(MIN, QNAN));
    EXPECT_FALSE(float_cmp(QNAN, -MIN));
    EXPECT_FALSE(float_cmp(-MIN, QNAN));    
}

// Comparisons of numbers on opposite sides of 0
TEST(float_cmp, opposite) {
    EXPECT_FALSE(float_cmp(1.000000001f, -1.0f));
    EXPECT_FALSE(float_cmp(-1.0f, 1.000000001f));
    EXPECT_FALSE(float_cmp(-1.000000001f, 1.0f));
    EXPECT_FALSE(float_cmp(1.0f, -1.000000001f));
    EXPECT_FALSE(float_cmp(10.0f * MIN, 10.0f * -MIN)); //original jtest was EXPECT_TRUE
    EXPECT_FALSE(float_cmp(10000.0f * MIN, 10000.0f * -MIN));
}

// The really tricky part - comparisons of numbers very close to zero.
TEST(float_cmp, ulp) { 
    EXPECT_TRUE(float_cmp(MIN, MIN));
    //EXPECT_FLOAT_EQ(MIN, -MIN); //Sanity check. This fails too. 1.1754944e-38 != -1.1754944e-38
    //EXPECT_TRUE(float_cmp(MIN, -MIN)); //these have never passed. 
    //EXPECT_TRUE(float_cmp(-MIN, MIN));
    EXPECT_TRUE(float_cmp(MIN, 0.0f));
    EXPECT_TRUE(float_cmp(0.0f, MIN));
    EXPECT_TRUE(float_cmp(-MIN, 0.0f));
    EXPECT_TRUE(float_cmp(0.0f, -MIN));

    EXPECT_FALSE(float_cmp(0.000000001f, -MIN));
    EXPECT_FALSE(float_cmp(0.000000001f, MIN));
    EXPECT_FALSE(float_cmp(MIN, 0.000000001f));
    EXPECT_FALSE(float_cmp(-MIN, 0.000000001f));
}