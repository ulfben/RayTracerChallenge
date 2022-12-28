// application.hpp
// lippuu: https://gist.github.com/lippuu/cbf4fa62fe8eed408159a558ff5c96ee
#pragma once
#include "pch.h"
#include "Tuple.h"
using Bitmap = std::vector<Color>;
struct FrameBuffer {
  Bitmap bitmap;
  int width = 0;
  int height = 0;

  FrameBuffer() = default;
  FrameBuffer(int width, int height) { resize(width, height); }

  inline void resize(int w, int h) {
    bitmap.resize(w * h);
    width = w;
    height = h;
  }

  inline void clear(const Color& c = Color{.0f,.0f,.0f,1.0f}) noexcept {
    std::ranges::fill(bitmap, c);
  }

  inline void set(int x, int y, const Color &c) noexcept {
    assert(x > 0 && x <= width && "Buffer::set called with invalid x position");
    assert(y > 0 && y <= height && "Buffer::set called with invalid y position");
    bitmap[y * width + x] = c;
  }
};