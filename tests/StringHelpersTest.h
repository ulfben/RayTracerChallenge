#pragma once
#include "../pch.h"
#include "../StringHelpers.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(StringHelper, get_lineHandlesEmptyInput) {
    const std::string_view data = ""sv;
    EXPECT_EQ(get_line(data, 0), ""sv);
    EXPECT_EQ(get_line(data, 1), ""sv);
}

TEST(StringHelper, get_lineReturnsEmptyOnOOB) {
    const std::string_view data = "line1\nline2\nline3\n";
    EXPECT_EQ(get_line(data, 7), ""sv);
}

TEST(StringHelper, get_lineIndexesCorrectly) {
    const std::string_view data = "line1\nline2\nline3\n";
    EXPECT_EQ(get_line(data, 0), "line1"sv);
    EXPECT_EQ(get_line(data, 1), "line2"sv);
    EXPECT_EQ(get_line(data, 2), "line3"sv); 
    EXPECT_EQ(get_line(data, 3), ""sv); 
}
TEST(StringHelper, get_lineIgnoresEmptyLines){
    const std::string_view data = "\n\n\n\nline1\n\n\n\nline2\n\n\n\n\n\n\n\n\n\n\n\n";
    EXPECT_EQ(get_line(data, 0), "line1"sv);
    EXPECT_EQ(get_line(data, 1), "line2"sv);
}

TEST(StringHelper, get_non_comment_line) {
    const std::string_view data = R"(P3
#4 3
#255
255 127 0 0 127 255 127 255 0 255 255 255
0 0 0 255 0 0 0 255 0 0 0 255
255 255 0 0 255 #255 255 0 255 127 127 127)";
    EXPECT_EQ(get_non_comment_line(data, 0), "P3"sv);
    EXPECT_EQ(get_non_comment_line(data, 1), "255 127 0 0 127 255 127 255 0 255 255 255"sv);
    EXPECT_EQ(get_non_comment_line(data, 2), "0 0 0 255 0 0 0 255 0 0 0 255"sv);
    EXPECT_EQ(get_non_comment_line(data, 3), "255 255 0 0 255 "sv); 
    EXPECT_EQ(get_non_comment_line(data, 4), ""sv); 
}

RESTORE_WARNINGS