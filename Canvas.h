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
    static constexpr auto CHANNELS = 3; // RGB
    static constexpr auto CHARS = 4;    //"255 "
    static constexpr auto PIXEL_CHAR_COUNT = CHANNELS * CHARS;
    static constexpr auto LINE_WRAP = 70 - PIXEL_CHAR_COUNT;

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

    constexpr void clear(const Color& col = Color{ .0f, .0f, .0f }) noexcept {
        std::ranges::fill(bitmap, col);
    }

    constexpr void set(size_type x, size_type y, const Color& col) noexcept {
        if (x < _width && y < _height) {
            bitmap[y * _width + x] = col;
        }
    }
    constexpr void set(const Point& p, const Color& col) noexcept {
        assert(p.x < std::numeric_limits<size_type>::max());
        assert(p.y < std::numeric_limits<size_type>::max());
        set(static_cast<size_type>(std::lroundf(p.x)), static_cast<size_type>(std::lroundf(p.y)), col);
    }
    constexpr Color get(size_type x, size_type y) const noexcept {
        assert(x <= _width && "Canvas::get called with invalid x position");
        assert(y <= _height && "Canvas::get called with invalid y position");
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
    
    std::string to_ppm() const {
        const size_type MAX_LINE_LENGTH = std::min(size_type(70), size_type(width()*PIXEL_CHAR_COUNT));
        std::string ppm = ppm_header();
        ppm.reserve(ppm.size() + (bitmap.size() * PIXEL_CHAR_COUNT));
        size_t line_length = 0;
        size_type pixelcount = 0;
        for (const auto& color : bitmap) {
            const std::string rgb = to_rgb_bytes(color);
            if (pixelcount == width() || line_length + rgb.size() >= MAX_LINE_LENGTH) {
                ppm.pop_back(); //remove trailing whitespace
                ppm.append(NEWLINE);
                line_length = 0;
                pixelcount = 0;
            }
            std::format_to(std::back_inserter(ppm), "{} ", rgb);
            line_length += rgb.size()+1;
            pixelcount++;
        }      
        ppm.pop_back(); //remove trailing whitespace
        ppm.append(NEWLINE); //always end with empty line
        return ppm;
    }

private:
    std::string ppm_header() const noexcept {
        return std::format("{}\n{} {}\n{}\n", PPM_VERSION, _width, _height, PPM_MAX);
    }
    bool must_wrap(auto fl, auto ls, auto pixelcount) const noexcept {
        const auto linelength = fl - ls;
        //const auto width_in_characters = width() * PIXEL_CHAR_COUNT;
        //linelength >= LINE_WRAP 
        return pixelcount >= width()-1 || pixelcount % width() == 0;
    }
};