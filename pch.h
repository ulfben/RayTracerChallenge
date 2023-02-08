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