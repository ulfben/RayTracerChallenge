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
#include <concepts>
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include <gtest/gtest.h>
#pragma warning(pop)

#pragma warning( disable : 26446 ) //"global" disabling of 'avoid subscript opterator, prefer gsl::at'

#define DISABLE_WARNINGS_FROM_GTEST \
    __pragma(warning(push))\
    __pragma(warning( disable : 26455 26426 26409 26477 26433 26818 26440 ))

#define RESTORE_WARNINGS __pragma(warning(pop))

using namespace std::string_view_literals;
using Real = float;
//static constexpr auto T_MISS = std::numeric_limits<Real>::max(); //magic value to denote an invalid t for intersections
static const std::vector<Real> MISS = {};// std::pair<Real, Real>(T_MISS, T_MISS);
static constexpr auto PPM_VERSION = "P3"sv;
static constexpr auto PPM_COMMENT = "#"sv;
static constexpr uint16_t PPM_MAX_LINE_LENGTH = 70;
static constexpr uint16_t PPM_MAX_BYTE_VALUE = 255; //max value of color components in PPM file. 
static constexpr bool RUN_SEQUENTIAL = false;
static constexpr unsigned SUGGESTED_THREAD_COUNT = 8; //only applicable is RUN_SEQUENTIAL = false. 

[[nodiscard]] bool empty(auto begin, auto end) noexcept {
  return std::distance(begin, end) == 0;
}

// narrow_cast(): a searchable way to do narrowing casts of values
template <class T, class U>
[[nodiscard]] constexpr T narrow_cast(U&& u) noexcept{
    return static_cast<T>(std::forward<U>(u));
}

constexpr size_t index_to_column(size_t index, size_t width) noexcept {        
    return index % width;
}
constexpr size_t index_to_row(size_t index, size_t width) noexcept {        
    return index / width;
}