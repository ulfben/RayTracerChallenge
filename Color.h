#pragma once
#include "pch.h"
#include "Math.h"

struct Color final {
    using value_type = Real;
    value_type r{};
    value_type g{};
    value_type b{};
};

constexpr Color color(Real r, Real g, Real b) noexcept {    
    return Color{ r, g, b };
}
constexpr Color color(Real rgb) noexcept {
    return color(rgb, rgb, rgb);
}

constexpr Color clamp(const Color& c, Color::value_type min = 0, Color::value_type max = 1) noexcept {
    return {
        std::clamp(c.r, min, max),
        std::clamp(c.g, min, max),
        std::clamp(c.b, min, max)
    };
}

//bytecolor is an optimization to help us speed up writing the image to files. 
//the idea is to bulk-convert the image-buffer into a smaller integer buffer, 
//which we can then split across threads to make strings out of.
struct ByteColor final {
    using value_type = uint8_t;
    static constexpr float MAX = PPM_MAX_BYTE_VALUE;
    value_type r{};
    value_type g{};
    value_type b{};
    constexpr ByteColor() noexcept = default;
    constexpr explicit ByteColor(const Color& c) noexcept {
        using math::map_to;
        r = map_to<value_type>(c.r);
        g = map_to<value_type>(c.g);
        b = map_to<value_type>(c.b);
        assert(r <= MAX && g <= MAX && b <= MAX);
        assert(r >= 0 && g >= 0 && b >= 0);
    };
};

constexpr ByteColor to_byte_color(const Color& col) noexcept {
    return ByteColor(col);
}

//sRGB conversion routines courtesy of http://www.ericbrasseur.org/gamma.html
//for explanation of the constants, see; https://en.wikipedia.org/wiki/SRGB
/*constexpr*/ Real sRGB_to_linear(Real s) noexcept {
    constexpr Real a = 0.055f;
    if (s <= 0.04045f) {
        return s / 12.92f;
    }
    return std::pow((s + a) / (1.0f + a), 2.4f);
}

/*constexpr*/ Color sRGB_to_linear(const Color& c) noexcept {
    return {sRGB_to_linear(c.r), sRGB_to_linear(c.g), sRGB_to_linear(c.b),};
}

/*constexpr*/ Real linear_to_sRGB(Real s) noexcept {
    constexpr Real a = 0.055f;
    if (s <= 0.0031308f) {
        return 12.92f * s;
    }
    return (1.0f + a) * std::pow(s, (1.0f / 2.4f)) - a;
}

/*constexpr*/ Color linear_to_sRGB(const Color& c) noexcept {
    return {linear_to_sRGB(c.r), linear_to_sRGB(c.g), linear_to_sRGB(c.b),};
}

/*constexpr*/ Real gamma_to_linear(Real s) noexcept {
    return std::pow(s, 2.2f);
}
/*constexpr*/ Real linear_to_gamma(Real s) noexcept {
    return std::pow(s, (1.0f / 2.2f));
}

//same as ByteColor, but additionally converts each pixel to the sRGB color space.
struct ByteColor_sRGB final {
    using value_type = uint8_t;
    value_type r{};
    value_type g{};
    value_type b{};
    constexpr ByteColor_sRGB() noexcept = default;
    /*constexpr*/ explicit ByteColor_sRGB(const Color& c) noexcept {        
        const auto srgb = ByteColor(linear_to_sRGB(clamp(c, 0.0f, 1.0f)));
        r = srgb.r;
        g = srgb.g;
        b = srgb.b;        
    };
};

/*constexpr*/ ByteColor_sRGB to_ByteColor_sRGB(const Color& col) noexcept {
    return ByteColor_sRGB(col);
}


static constexpr auto BLACK = Color{ 0, 0, 0 };
static constexpr auto WHITE = Color{1, 1, 1};
static constexpr auto RED = Color{1, 0, 0};
static constexpr auto GREEN = Color{0, 1, 0};
static constexpr auto BLUE = Color{0, 0, 1};
static constexpr auto MAGENTA = Color{ 1, 0, 1 };

//Color interface
constexpr Color operator*(const Color& lhs, const Color& rhs) noexcept {
    return { lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b }; //hadamard product
}
constexpr Color operator*(const Color& lhs, Real scalar) noexcept {
    return Color{ lhs.r * scalar, lhs.g * scalar, lhs.b * scalar };
}
constexpr void operator*=(Color& lhs,Real scalar) noexcept {
    lhs.r *= scalar;
    lhs.g *= scalar;
    lhs.b *= scalar;
}
constexpr Color operator/(const Color& lhs, Real scalar) noexcept {
    return Color{ lhs.r / scalar, lhs.g / scalar, lhs.b / scalar };
}
constexpr Color operator+(const Color& lhs, Color rhs) noexcept {
    return Color{ lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b };
}
constexpr Color operator-(const Color& lhs, Color rhs) noexcept {
    return Color{ lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b };
}
constexpr bool operator==(const Color& lhs, const Color& rhs) noexcept {
    using math::float_cmp;
    constexpr auto epsilon = 0.0005f;
    return float_cmp(lhs.r, rhs.r, epsilon) && float_cmp(lhs.g, rhs.g, epsilon) &&
        float_cmp(lhs.b, rhs.b, epsilon);
};
constexpr Color lerp(Color start, Color end, Real t) {    
    //return (start*(1.0f - t)) + (end*t);
    return start + (end - start) * t;
}

/*constexpr*/ void to_sRGB(Color& c) noexcept {
    c = linear_to_sRGB(clamp(c, 0.0f, 1.0f));
}

void to_sRGB(std::span<Color> buffer) noexcept {
    std::for_each(std::execution::par, buffer.begin(), buffer.end(),
        [] (auto& color) constexpr -> void { to_sRGB(color); }
    );
}

//print and string features
#pragma warning(push)
#pragma warning( disable : 26481 ) //spurious warning; "don't use pointer arithmetic" 
std::ostream& operator<<(std::ostream& os, const Color& t) {
    os << std::format("{}, {}, {}"sv, t.r, t.g, t.b);
    return os;
}

std::string to_string(const Color& c) {
    return std::format("{} {} {}"sv, c.r, c.g, c.b);
}
std::string to_string(const ByteColor& c) {
    return std::format("{} {} {}"sv, c.r, c.g, c.b);
}
std::string to_string(const ByteColor_sRGB& c) {
    return std::format("{} {} {}"sv, c.r, c.g, c.b);
}
std::string to_string_with_trailing_space(const ByteColor& c) {
    return std::format("{} {} {} "sv, c.r, c.g, c.b);
}
std::string to_string_with_trailing_space(const ByteColor_sRGB& c) {
    return std::format("{} {} {} "sv, c.r, c.g, c.b);
}
#pragma warning(pop)