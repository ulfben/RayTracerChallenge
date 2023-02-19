#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Canvas.h"
#include "../StringHelpers.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(Canvas, hasWidthHeightAndColor) {
  const auto canvas = Canvas(10, 20);
  EXPECT_EQ(canvas.width(), (Canvas::size_type)10);
  EXPECT_EQ(canvas.height(), (Canvas::size_type)20);
  for (const auto &pixel : canvas) {
    EXPECT_EQ(pixel, color(.0f, .0f, .0f));
  }
}

TEST(Canvas, canSetAndGetPixel) {
  auto canvas = Canvas(10u, 20u);
  const auto red = color(1, 0, 0);
  canvas.set(2, 3, red);
  EXPECT_EQ(canvas.get(2, 3), red);
}

TEST(Canvas, canOutputPPMHeader) {
  const auto canvas = Canvas(5, 3);
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  ASSERT_GE(lines.size(), 3);
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
  ASSERT_EQ(lines.size(), 6);
  EXPECT_EQ(lines[3], "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0"sv);
  EXPECT_EQ(lines[4], "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0"sv);
  EXPECT_EQ(lines[5], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255"sv);
}

TEST(Canvas, PPMIsMax70CharsPerLine) {
  auto canvas = Canvas(10, 2);
  canvas.clear(color(1.0f, 0.8f, 0.6f));
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");  
  for (auto i = 3; i < lines.size(); i++) { //start at 3 to skip header.
      EXPECT_LT(lines[i].size(), 70);     
  }
}

TEST(Canvas, PPMLineWrapDoesntBreakPixels) {
  auto canvas = Canvas(10, 2);
  canvas.clear(color(0.0f, 0.8f, 0.6f)); //0 204 153
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n"); 
  for (auto i = 3; i < lines.size(); i++) {
      EXPECT_LT(lines[i].size(), 70);
      EXPECT_TRUE(lines[i].starts_with("0 204 153"sv));
      EXPECT_TRUE(lines[i].ends_with("0 204 153"sv));
  }
}

TEST(Canvas, PPMLineWrapDoesntBreakPixels2) {
  auto canvas = Canvas(10, 2);
  canvas.clear(color(0.0f, 0.2f, 0.6f)); //0 51 153
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  for (auto i = 3; i < lines.size(); i++) {
      EXPECT_LT(lines[i].size(), 70);
      EXPECT_TRUE(lines[i].starts_with("0 51 153"sv));
      EXPECT_TRUE(lines[i].ends_with("0 51 153"sv));
  }
}

TEST(Canvas, PPMIsTerminatedByNewline) {
  auto canvas = Canvas(5, 3);
  const auto output = canvas.to_ppm();
  EXPECT_EQ('\n', output.back());
}

RESTORE_WARNINGS