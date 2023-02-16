#pragma once
#include "pch.h"
#include "Tuple.h"
#include "StringHelpers.h"


// application.hpp
// lippuu: https://gist.github.com/lippuu/cbf4fa62fe8eed408159a558ff5c96ee
using Bitmap = std::vector<Color>;
class Canvas final {
  Bitmap bitmap;
  size_t _width = 0;
  size_t _height = 0;

public:
  using size_type = uint16_t;

  Canvas(size_type width, size_type height) {
    resize(width, height);
    clear();
  }

  constexpr void resize(size_type w, size_type h) {
    bitmap.resize(static_cast<size_t>(w) * h);
    _width = w;
    _height = h;
  }

  constexpr void clear(const Color &col = Color{.0f, .0f, .0f, 1.0f}) noexcept {
    std::ranges::fill(bitmap, col);
  }

  constexpr void set(size_type x, size_type y, const Color &col) noexcept {
    if (x < _width && y < _height) {
      bitmap[y * _width + x] = col;
    }
  }
  constexpr void set(const Point& p, const Color &col) noexcept { 
    assert(p.x < std::numeric_limits<size_type>::max());
    assert(p.y < std::numeric_limits<size_type>::max());
    set(static_cast<size_type>(std::lroundf(p.x)), static_cast<size_type>(std::lroundf(p.y)), col);
  }
  constexpr Color get(size_type x, size_type y) const noexcept {    
      assert(x <= _width && "Buffer::get called with invalid x position");
      assert(y <= _height && "Buffer::get called with invalid y position");  
      return bitmap[y * _width + x];    
  }
  constexpr size_type width() const noexcept {
    return static_cast<size_type>(_width);
  }
  constexpr size_type height() const noexcept {
    return static_cast<size_type>(_height);
  }
  constexpr Real widthf() const noexcept {
    return static_cast<Real>(_width);
  }
  constexpr Real heightf() const noexcept {
    return static_cast<Real>(_height);
  }
  constexpr auto begin() const noexcept { return bitmap.begin(); }
  constexpr auto end() const noexcept { return bitmap.end(); }
  
  std::string to_ppm() const noexcept {
    using namespace std::string_literals;
    const auto CHANNELS = 3; // RGB
    const auto CHARS = 4;    //"255 "
    const auto PIXEL_CHAR_COUNT = CHANNELS * CHARS;
    const auto LINE_WRAP = 70-PIXEL_CHAR_COUNT;
    std::string ppm = ppm_header();    
    ppm.reserve(ppm.size() + (bitmap.size() * PIXEL_CHAR_COUNT));
    for (size_t y = 0; y < _height; ++y) {
      auto linestart = ppm.size();
      for (size_t x = 0; x < _width; ++x) {
        std::format_to(std::back_inserter(ppm), "{} ",
                       to_rgb_bytes(bitmap[y * _width + x]));
        if(ppm.size()-linestart >= LINE_WRAP){          
          ppm.pop_back(); //remove trailing whitespace
          ppm.append(NEWLINE);
          linestart = ppm.size();
        }
      }
      ppm.pop_back(); //remove trailing whitespace
      ppm.append(NEWLINE);
    }
    ppm.shrink_to_fit();
    return ppm;
  }

private:
  std::string ppm_header() const noexcept {
    return std::format("{}\n{} {}\n{}\n", PPM_VERSION, _width, _height, PPM_MAX);
  }
};