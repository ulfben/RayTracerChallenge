// application.hpp
// lippuu: https://gist.github.com/lippuu/cbf4fa62fe8eed408159a558ff5c96ee
#pragma once
#include "pch.h"
#include "Tuple.h"
#include "StringHelpers.h"

using Bitmap = std::vector<Color>;
class Canvas {
  Bitmap bitmap;
  size_t _width = 0;
  size_t _height = 0;

public:
  Canvas(uint32_t width, uint32_t height) {
    resize(width, height);
    clear();
  }

  constexpr void resize(uint32_t w, uint32_t h) {
    bitmap.resize(static_cast<size_t>(w) * h);
    _width = w;
    _height = h;
  }

  constexpr void clear(const Color &c = Color{.0f, .0f, .0f, 1.0f}) noexcept {
    std::ranges::fill(bitmap, c);
  }

  constexpr void set(uint32_t x, uint32_t y, const Color &c) noexcept {
    assert(x <= _width && "Buffer::set called with invalid x position");
    assert(y <= _height && "Buffer::set called with invalid y position");
    bitmap[y * _width + x] = c;
  }
  constexpr Color get(uint32_t x, uint32_t y) const noexcept {
    assert(x <= _width && "Buffer::get called with invalid x position");
    assert(y <= _height && "Buffer::get called with invalid y position");
    return bitmap[y * _width + x];
  }
  constexpr uint32_t width() const noexcept {
    return static_cast<uint32_t>(_width);
  }
  constexpr uint32_t height() const noexcept {
    return static_cast<uint32_t>(_height);
  }
  constexpr auto begin() const noexcept { return bitmap.begin(); }
  constexpr auto end() const noexcept { return bitmap.end(); }
  std::string to_ppm() const noexcept {
    using namespace std::string_literals;
    const auto CHANNELS = 4; // RGBA
    const auto CHARS = 4;    //"255 "
    std::string result = ppm_header();
    result.reserve(result.size() + (_width * _height * (CHANNELS * CHARS)));
    for (size_t y = 0; y < _height; ++y) {
      for (size_t x = 0; x < _width; ++x) {
        std::format_to(std::back_inserter(result), "{} ",
                       to_rgb_bytes(bitmap[y * _width + x]));
      }
      result.pop_back(); //remove trailing whitespace
      result.append("\n"s);
    }
    return result;
  }

private:
  std::string ppm_header() const noexcept {
    return std::format("{}\n{} {}\n{}\n", PPM_VERSION, _width, _height, PPM_MAX);
  }
};