#pragma once
#include <type_traits>
#include <cassert>
#include <limits>
#include <algorithm>
#include <numeric>
#include <vector>
#include <format>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <execution>
#include <functional>
#include <codeanalysis\warnings.h>

#pragma warning(push)
//#pragma warning( disable : 26495 )
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include <gtest/gtest.h>
#pragma warning(pop)

#pragma warning( disable : 26446 ) //"global" disabling of 'avoid subscript opterator, prefer gsl::at'

#define DISABLE_WARNINGS_FROM_GTEST \
    __pragma(warning(push))\
    __pragma(warning( disable : 26455 26426 26409 26477 26433 26818 26440 ))

#define RESTORE_WARNINGS __pragma(warning(pop))

using namespace std::string_view_literals;
struct Tuple;
using Real = float;
using Vector = Tuple;
using Point = Tuple;
static constexpr auto PPM_VERSION = "P3"sv;
static constexpr uint16_t PPM_MAX_LINE_LENGTH = 70;
static constexpr uint16_t PPM_MAX_BYTE_VALUE = 255; //max value of color components in PPM file. 

[[nodiscard]] bool empty(auto begin, auto end) noexcept {
  return std::distance(begin, end) == 0;
}

// narrow_cast(): a searchable way to do narrowing casts of values
template <class T, class U>
[[nodiscard]] constexpr T narrow_cast(U&& u) noexcept{
    return static_cast<T>(std::forward<U>(u));
}