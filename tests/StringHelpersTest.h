#pragma once
#include "../pch.h"
#include "../StringHelpers.h"

DISABLE_WARNINGS_FROM_GTEST

TEST(StringHelper, getLineHandlesEmptyInput) {
    const std::string_view data = ""sv;
    EXPECT_EQ(get_line(data, 0), ""sv);
    EXPECT_EQ(get_line(data, 1), ""sv);
}

TEST(StringHelper, getLineReturnsEmptyOnOOB) {
    const std::string_view data = "line1\nline2\nline3\n";
    EXPECT_EQ(get_line(data, 7), ""sv);
}

//TODO: how do we handle empty lines? 
//TODO: how de handle trailing 

//TEST(StringHelper, getLineReturnsEmptyOnOOB) {
//    const std::string_view data = "line1line2line3";
//    EXPECT_EQ(get_line(data, 7), ""sv);
//}

TEST(StringHelper, getLineIndexesCorrectly) {
    const std::string_view data = "line1\nline2\nline3\n";
    EXPECT_EQ(get_line(data, 0), "line1"sv);
    EXPECT_EQ(get_line(data, 1), "line2"sv);
    EXPECT_EQ(get_line(data, 2), "line3"sv); 
    EXPECT_EQ(get_line(data, 3), ""sv); 
}


RESTORE_WARNINGS