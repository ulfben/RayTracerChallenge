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


std::size_t length_of_all_elements(auto begin, auto end) noexcept{
    std::size_t totalLength{};
    for(auto i = begin; i != end; ++i){
        totalLength += i->length();
    }
    return totalLength;
}

//a join for std::string_view
template<std::input_iterator InputIterator>
    requires std::same_as<std::string_view, typename std::iterator_traits<InputIterator>::value_type>
std::string join(InputIterator begin, InputIterator end, const std::string& delimiter = ", "s) {
    if (begin == end) return {}; // Early return for empty range    
    const auto count = std::distance(begin, end);    
    const std::size_t totalLength = length_of_all_elements(begin, end) + 
                                    (count > 1 ? (delimiter.length() * (count - 1)) : 0);    
    std::string result;
    result.reserve(totalLength);
    result += std::string(*begin++); // Add the first element without a leading delimiter
    while (begin != end) {
        result += delimiter + std::string(*begin++);
    }
    return result;
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

std::string_view get_line(std::string_view sv, size_t n, std::string_view line_ending = "\n"sv) {
    size_t line_start = 0;
    size_t line_end = 0;
    for (size_t i = 0; i <= n; ++i) {
        // If we are looking for the first line or have found the start of the nth line
        if (i == n) {
            line_end = sv.find(line_ending, line_start);            
            if (line_end == std::string_view::npos) {// If line_end is npos, we're at the last line or beyond  
                if (i != 0 && line_start >= sv.size()) {
                    return {}; // If this isn't the first line and we are at the end of the string, return empty
                }                
                line_end = sv.size();
            }
            // Return the substring representing the line
            return sv.substr(line_start, line_end - line_start);
        }

        // Find the next newline character to advance line_start for the next iteration
        size_t next_newline = sv.find('\n', line_start);
        // If there are no more newlines, break early
        if (next_newline == std::string_view::npos) {
            return {};
        }
        line_start = next_newline + 1;
    }
    return {};
}