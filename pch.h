#pragma once
#include <type_traits>
#include <cassert>
#include <limits>
#include <algorithm>
#include <numeric> //std::accumulate
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

static constexpr auto PPM_VERSION = "P3"sv;
static constexpr auto PPM_MAX = 255; //max value of color components in PPM file. 

[[nodiscard]] bool empty(auto begin, auto end) noexcept {
  return std::distance(begin, end) == 0;
}