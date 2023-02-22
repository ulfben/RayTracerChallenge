#pragma once
#include "pch.h"
#include "Tuple.h"
#include "StringHelpers.h"

//A neat API example by lippuu: https://gist.github.com/lippuu/cbf4fa62fe8eed408159a558ff5c96ee
using Bitmap = std::vector<Color>;
class Canvas final {
public:
    using size_type = uint16_t;
    static constexpr size_type CHANNELS = 3; //RGB
    static constexpr size_type CHARS = 4;    //"255 "
    static constexpr size_type CHARS_PER_PIXEL = CHANNELS * CHARS;   

    constexpr Canvas(size_type width, size_type height) {
        resize(width, height);
        clear();
    }

    constexpr void resize(size_type w, size_type h) {
        assert(w > 0 && h > 0 && "Canvas dimensions must be non-zero.");
        bitmap.resize(static_cast<size_t>(w) * h);
        _width = w;
        _height = h;
    }

    constexpr void clear(const Color& col = Color{ .0f, .0f, .0f }) noexcept {
        std::ranges::fill(bitmap, col);
    }       
    constexpr void set(size_type i, const Color& col) noexcept {
        if (i < bitmap.size()) {            
            bitmap[i] = col;
        }
    }
    constexpr void set(size_type x, size_type y, const Color& col) noexcept {
        if (x < _width && y < _height) {
            const auto row = static_cast<size_t>(y) * _width;
            bitmap[row + x] = col;
        }
    }
    constexpr void set(const Point& p, const Color& col) noexcept {
        assert(p.x < std::numeric_limits<size_type>::max());
        assert(p.y < std::numeric_limits<size_type>::max());
        set(narrow_cast<size_type>(std::lroundf(p.x)), narrow_cast<size_type>( std::lroundf(p.y)), col);
    }
    constexpr Color get(size_type x, size_type y) const noexcept {
        assert(x <= _width && "Canvas::get called with invalid x position");
        assert(y <= _height && "Canvas::get called with invalid y position");
        const auto row = static_cast<size_t>( y ) *_width;
        return bitmap[row + x];
    }
    constexpr size_type width() const noexcept {
        return _width;
    }
    constexpr size_type height() const noexcept {
        return _height;
    }
    constexpr Real widthf() const noexcept {
        return static_cast<Real>(_width);
    }
    constexpr Real heightf() const noexcept {
        return static_cast<Real>(_height);
    }
    constexpr auto data() const noexcept { return bitmap.data(); }
    constexpr auto begin() const noexcept { return bitmap.begin(); }
    constexpr auto end() const noexcept { return bitmap.end(); }
    constexpr auto size() const noexcept { return bitmap.size(); }

    std::string to_ppm() const {
        std::string ppm = ppm_header();
        line_state state(width());
        for (const auto& color : bitmap) {
            const std::string rgb = to_string(to_byte_color(color));
            if (state.should_wrap(rgb.size())) {
                state.wrap(ppm);
            }
            #pragma warning( suppress : 26481 ) //spurious warning; "don't use pointer arithmetic" 
            ppm.append(std::format("{} "sv, rgb));
            state.added(rgb.size() + 1);
        }
        state.wrap(ppm); //PPM always ends on a newline.
        return ppm;
    }
    
    //optimized path, under construction
    std::string to_ppm2() const {
        const std::vector<ByteColor> buffer(bitmap.begin(), bitmap.end());    
        std::string ppm = ppm_header();
        line_state state(width());
        for (const auto& color : buffer) {
            const std::string rgb = to_string(color);
            if (state.should_wrap(rgb.size())) {
                state.wrap(ppm);
            }
            #pragma warning( suppress : 26481 ) //spurious warning; "don't use pointer arithmetic" 
            ppm.append(std::format("{} "sv, rgb));
            state.added(rgb.size() + 1);
        }
        state.wrap(ppm); //PPM always ends on a newline.
        return ppm;
    }

private:
    Bitmap bitmap;
    size_type _width = 0;
    size_type _height = 0;

    std::string ppm_header() const {
        #pragma warning( suppress : 26481 ) //spurious warning; "don't use pointer arithmetic" 
        return std::format("{}\n{} {}\n{}\n"sv, PPM_VERSION, _width, _height, PPM_MAX_BYTE_VALUE);
    }
    struct line_state {
        size_type bitmap_width;
        size_t char_count = 0;
        size_type pixel_count = 0;
        explicit constexpr line_state(size_type bmp_width) noexcept : bitmap_width(bmp_width) {};
        constexpr size_type max_length(size_type width) const noexcept {
            return std::min(static_cast<size_type>(width * CHARS_PER_PIXEL), PPM_MAX_LINE_LENGTH);
        }
        constexpr bool should_wrap(size_t newchars) const noexcept {
            return (pixel_count == bitmap_width || char_count + newchars >= max_length(bitmap_width));
        }
        constexpr void wrap(std::string& s) {
            s.pop_back(); //remove trailing whitespace
            s.append(NEWLINE);
            char_count = 0;
            pixel_count = 0;
        }
        constexpr void added(size_t newchars) noexcept {
            char_count += newchars;
            pixel_count++;
        }
    };
};

void save_to_file(const Canvas& img, std::string_view path) {
    std::ofstream ofs(path.data(), std::ofstream::out);
    ofs << img.to_ppm2();
}