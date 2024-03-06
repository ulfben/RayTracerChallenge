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
  EXPECT_EQ(lines[3], "254 0 0 0 0 0 0 0 0 0 0 0 0 0 0"sv);
  EXPECT_EQ(lines[4], "0 0 0 0 0 0 0 187 0 0 0 0 0 0 0"sv); //0.5f = 127 (linear) = 187 (srgb)
  EXPECT_EQ(lines[5], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 254"sv); //TODO: why are we not hitting 255 after srgb conversion
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
  const auto linearColor = color(0.0f, 0.8f, 0.6f);  //0 204 153
  const auto sRGBString = to_string(to_ByteColor_sRGB(linearColor));
  canvas.clear(linearColor); 
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n"); 
  for (auto i = 3; i < lines.size(); i++) {
      EXPECT_LT(lines[i].size(), 70);
      EXPECT_TRUE(lines[i].starts_with(sRGBString));
      EXPECT_TRUE(lines[i].ends_with(sRGBString));
  }
}

TEST(Canvas, PPMLineWrapDoesntBreakPixels2) {
  auto canvas = Canvas(10, 2);
  const auto linearColor = color(0.0f, 0.2f, 0.6f);  
  const auto sRGBString = to_string(to_ByteColor_sRGB(linearColor));
  canvas.clear(linearColor); //0 51 153

  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  for (auto i = 3; i < lines.size(); i++) {
      EXPECT_LT(lines[i].size(), 70);
      EXPECT_TRUE(lines[i].starts_with(sRGBString));
      EXPECT_TRUE(lines[i].ends_with(sRGBString));
  }
}

TEST(Canvas, PPMHandlesTinyCanvas) {
  auto canvas = Canvas(1, 1);
  const auto linearColor = color(0.0f, 0.2f, 0.6f);  
  const auto sRGBString = to_string(to_ByteColor_sRGB(linearColor));
  canvas.clear(linearColor); //0 51 153
  const auto output = canvas.to_ppm();
  const auto lines = split(output, "\n");
  for (auto i = 3; i < lines.size(); i++) {
      EXPECT_LT(lines[i].size(), 70);
      EXPECT_TRUE(lines[i].starts_with(sRGBString));
      EXPECT_TRUE(lines[i].ends_with(sRGBString));
  }
}

TEST(Canvas, PPMIsTerminatedByNewline) {
  auto canvas = Canvas(5, 3);
  const auto output = canvas.to_ppm();
  EXPECT_EQ('\n', output.back());
}


TEST(Canvas, FromPPMReturnsNulloptForBadMagicNumber) {
  auto ppm = 
R"(P32
1 1
255
0 0 0)";
  
  const auto c = canvas_from_ppm(ppm);
  EXPECT_EQ(c, std::nullopt);
}

TEST(Canvas, FromPPMReturnsCanvasOfCorrectDimension) {
  auto ppm = 
R"(P3
10 2
255
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)";
  
  const auto c = canvas_from_ppm(ppm);  
  const auto& result = c.value();
  EXPECT_EQ(result.width(), 10);
  EXPECT_EQ(result.height(), 2);
}

TEST(Canvas, FromPPMReadsPixelData) {
  auto ppm = 
R"(P3
4 3
255
255 127 0 0 127 255 127 255 0 255 255 255
0 0 0 255 0 0 0 255 0 0 0 255
255 255 0 0 255 255 255 0 255 127 127 127)";
  
  const auto c = canvas_from_ppm(ppm); 
  const auto& result = c.value();
  EXPECT_EQ(result.width(), 4);
  EXPECT_EQ(result.height(), 3);
  EXPECT_EQ(result.get(0, 0), color(1.0f, 0.498f, 0.0f));
  EXPECT_EQ(result.get(1, 0), color(0, 0.498f, 1.0f));
  EXPECT_EQ(result.get(2, 0), color(0.498f, 1.0f, 0.0f));
  EXPECT_EQ(result.get(3, 0), color(1.0f, 1.0f, 1.0f));
  EXPECT_EQ(result.get(0, 1), color(0, 0, 0));
  EXPECT_EQ(result.get(1, 1), color(1.0f, 0.0f, 0.0f));
  EXPECT_EQ(result.get(2, 1), color(0.0f, 1.0f, 0.0f));
  EXPECT_EQ(result.get(3, 1), color(0.0f, 0.0f, 1.0f));
  EXPECT_EQ(result.get(0, 2), color(0.0f, 1.0f, 0.0f));
  EXPECT_EQ(result.get(1, 2), color(0.0f, 1.0f, 1.0f));
  EXPECT_EQ(result.get(2, 2), color(1.0f, 0.0f, 1.0f));
  EXPECT_EQ(result.get(3, 2), color(0.498f, 0.498f, 0.498f));
}

RESTORE_WARNINGS