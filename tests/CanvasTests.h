#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Canvas.h"
#include "../StringHelpers.h"

TEST(Canvas, hasWidthHeightAndColor) {
  const auto canvas = Canvas(10, 20);
  EXPECT_EQ(canvas.width(), (uint32_t)10);
  EXPECT_EQ(canvas.height(), (uint32_t)20);
  for (const auto &pixel : canvas) {
    EXPECT_EQ(pixel, color(.0f, .0f, .0f));
  }
}

TEST(Canvas, canSetAndGetPixel) {
  auto canvas = Canvas(10, 20);
  const auto red = color(1, 0, 0);
  canvas.set(2, 3, red);
  EXPECT_EQ(canvas.get(2, 3), red);
}

TEST(Canvas, canOutputPPMHeader) {
  const auto canvas = Canvas(5, 3);
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  EXPECT_EQ(lines[0], "P3"sv);
  EXPECT_EQ(lines[1], "5 3"sv);
  EXPECT_EQ(lines[2], "255"sv);
}

TEST(Canvas, canOutputPPMPixelData) {
  auto canvas = Canvas(5, 3);
  canvas.set(0, 0, color(1.5f, 0, 0));
  canvas.set(2, 1, color(0, 0.5f, 0));
  canvas.set(4, 2, color(-0.5, 0, 1.0f));
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  EXPECT_EQ(lines[3], "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0"sv);
  EXPECT_EQ(lines[4], "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0"sv);
  EXPECT_EQ(lines[5], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255"sv);
}

TEST(Canvas, PPMIsMax70CharsPerLine) {
  auto canvas = Canvas(10, 2);
  canvas.clear(color(1.0f, 0.8f, 0.6f));
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  const auto LESS_THAN_70_CHARS =
      "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153"sv;
  const auto WRAPPED =
      "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153"sv;

  EXPECT_EQ(lines[3].size(), LESS_THAN_70_CHARS.size());
  EXPECT_EQ(lines[4].size(), WRAPPED.size());
  EXPECT_EQ(lines[5].size(), LESS_THAN_70_CHARS.size());
  EXPECT_EQ(lines[6].size(), WRAPPED.size());

  EXPECT_EQ(lines[3], LESS_THAN_70_CHARS);
  EXPECT_EQ(lines[4], WRAPPED);
  EXPECT_EQ(lines[5], LESS_THAN_70_CHARS);
  EXPECT_EQ(lines[6], WRAPPED);
}

TEST(Canvas, PPMLineWrapDoesntBreakPixels) {
  auto canvas = Canvas(10, 2);
  canvas.clear(color(0.0f, 0.8f, 0.6f));
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  const auto LESS_THAN_70_CHARS =
      "0 204 153 0 204 153 0 204 153 0 204 153 0 204 153 0 204 153"sv;
  const auto WRAPPED = "0 204 153 0 204 153 0 204 153 0 204 153"sv;

  EXPECT_EQ(lines[3].size(), LESS_THAN_70_CHARS.size());
  EXPECT_EQ(lines[4].size(), WRAPPED.size());
  EXPECT_EQ(lines[5].size(), LESS_THAN_70_CHARS.size());
  EXPECT_EQ(lines[6].size(), WRAPPED.size());

  EXPECT_EQ(lines[3], LESS_THAN_70_CHARS);
  EXPECT_EQ(lines[4], WRAPPED);
  EXPECT_EQ(lines[5], LESS_THAN_70_CHARS);
  EXPECT_EQ(lines[6], WRAPPED);
}

TEST(Canvas, PPMLineWrapDoesntBreakPixels2) {
  auto canvas = Canvas(10, 2);
  canvas.clear(color(0.0f, 0.2f, 0.6f));
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");

  const auto LESS_THAN_70_CHARS =
      "0 51 153 0 51 153 0 51 153 0 51 153 0 51 153 0 51 153 0 51 153"sv;
  const auto WRAPPED = "0 51 153 0 51 153 0 51 153"sv;

  EXPECT_EQ(lines[3].size(), LESS_THAN_70_CHARS.size());
  EXPECT_EQ(lines[4].size(), WRAPPED.size());
  EXPECT_EQ(lines[5].size(), LESS_THAN_70_CHARS.size());
  EXPECT_EQ(lines[6].size(), WRAPPED.size());

  EXPECT_EQ(lines[3], LESS_THAN_70_CHARS);
  EXPECT_EQ(lines[4], WRAPPED);
  EXPECT_EQ(lines[5], LESS_THAN_70_CHARS);
  EXPECT_EQ(lines[6], WRAPPED);
}

TEST(Canvas, PPMIsTerminatedByNewline) {
  auto canvas = Canvas(5, 3);
  const auto output = canvas.to_ppm();
  EXPECT_EQ('\n', output.back());
}