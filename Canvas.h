// application.hpp
// lippuu: https://gist.github.com/lippuu/cbf4fa62fe8eed408159a558ff5c96ee
#pragma once
#include "Tuple.h"
#include "pch.h"
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
    return std::format("{}\n{} {}\n255\n", "P3", _width, _height);
  }
};