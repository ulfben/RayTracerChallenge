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
static constexpr string_view NEWLINE = "\n"sv;

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

std::string join(std::input_iterator auto begin, std::input_iterator auto end,
                 const std::string& delim = ", "s) {
  if (empty(begin, end)) {
    return {};
  }
  using namespace std;
  // std::accumulate until we get std::experimental::make_ostream_joiner, or
  // std::format::join
  return std::accumulate(begin + 1, end, to_string(*begin),
                         [&delim](const string &a, auto b) {
                           return a + delim + to_string(b);
                         });
}

#include <charconv>
#include <optional>
template<typename Number>
[[nodiscard]] std::optional<Number> from_chars(std::string_view s) noexcept {
    Number value{};
    auto res = std::from_chars(s.data(), s.data() + s.size(), value);    
    if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) {
        return std::nullopt;
    }    
    return value;
}