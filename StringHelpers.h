#pragma once
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>
#include <string>
#include <string_view>

using namespace std::string_view_literals;
using namespace std::string_literals;
using std::string_view;
using StringViews = std::vector<string_view>;
static constexpr string_view WHITE_SPACE = " \f\n\r\t\v"sv;

string_view trimLeft(string_view in, string_view delims = WHITE_SPACE) noexcept {
  const auto countFromTheLeft = in.find_first_not_of(delims);
  in.remove_prefix(countFromTheLeft);
  return in;
};
string_view trimRight(string_view in, string_view delims = WHITE_SPACE) noexcept {
  const auto lastValidPos = in.find_last_not_of(delims);
  const auto countFromTheRight = in.size() - lastValidPos - 1;
  in.remove_suffix(countFromTheRight);
  return in;
};

string_view trim(string_view in, string_view delims = WHITE_SPACE) noexcept {
  return trimRight(trimLeft(in, delims), delims);
}

[[nodiscard]] StringViews split(string_view source, string_view delim) {
  StringViews values;  
  for (std::ranges::split_view outer_view{source, delim}; 
        const auto &split : outer_view) {
    if (split.empty()) {
      continue;
    }
    values.emplace_back(split.begin(), split.end());
  }
  return values;
}