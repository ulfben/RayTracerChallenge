#pragma once
#include "pch.h"
#include "Math.h"

//sRGB conversion routines courtesy of http://www.ericbrasseur.org/gamma.html
//for explanation of the constants, see; https://en.wikipedia.org/wiki/SRGB
/*constexpr*/ Real sRGB_to_linear(Real s) noexcept {
    constexpr Real a = 0.055f;
    if (s <= 0.04045f) {
        return s / 12.92f;
    }
    return std::pow((s + a) / (1.0f + a), 2.4f);
}
/*constexpr*/ Real linear_to_sRGB(Real s) noexcept {
    constexpr Real a = 0.055f;
    if (s <= 0.0031308f) {
        return 12.92f * s;
    }
    return (1.0f + a) * std::pow(s, (1.0f / 2.4f)) - a;
}

/*constexpr*/ Real gamma_to_linear(Real s) noexcept {
    return std::pow(s, 2.2f);
}
/*constexpr*/ Real linear_to_gamma(Real s) noexcept {
    return std::pow(s, (1.0f / 2.2f));
}

struct Color final {
    Real r{};
    Real g{};
    Real b{};
    constexpr Color operator*(const Color& that) const noexcept {
        return { r * that.r, g * that.g, b * that.b }; //hadamard product
    }
    constexpr Color operator*(Real scalar) const noexcept {
        return Color{ r * scalar, g * scalar, b * scalar };
    }
    constexpr void operator*=(Real scalar) noexcept {
        r *= scalar;
        g *= scalar;
        b *= scalar;
    }
    constexpr Color operator/(Real scalar) const noexcept {
        return Color{ r / scalar, g / scalar, b / scalar };
    }
    constexpr Color operator+(Color rhs) const noexcept {
        return Color{ r + rhs.r, g + rhs.g, b + rhs.b };
    }
    constexpr Color operator-(Color rhs) const noexcept {
        return Color{ r - rhs.r, g - rhs.g, b - rhs.b };
    }
    constexpr bool operator==(const Color& rhs) const noexcept {
        using math::float_cmp;
        constexpr auto epsilon = 0.0005f;
        return float_cmp(r, rhs.r, epsilon) && float_cmp(g, rhs.g, epsilon) &&
            float_cmp(b, rhs.b, epsilon);
    };
};
constexpr Color color(Real r, Real g, Real b) noexcept {
    return Color{ r, g, b };
}
constexpr Color color(Real rgb) noexcept {
    return color(rgb, rgb, rgb);
}

void to_sRGB(Color& c) noexcept {
    c.r = linear_to_sRGB(std::clamp(c.r, 0.0f, 1.0f));
    c.g = linear_to_sRGB(std::clamp(c.g, 0.0f, 1.0f));
    c.b = linear_to_sRGB(std::clamp(c.b, 0.0f, 1.0f));
}

void to_sRGB(std::span<Color> buffer) noexcept {
    std::for_each(std::execution::par, buffer.begin(), buffer.end(),
        [](auto& color) { to_sRGB(color); }
    );
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

//same as ByteColor, but additionally converts each pixel to the sRGB color space.
struct ByteColor_sRGB final {
    using value_type = uint8_t;
    static constexpr float MAX = PPM_MAX_BYTE_VALUE;
    value_type r{};
    value_type g{};
    value_type b{};
    constexpr ByteColor_sRGB() noexcept = default;
    constexpr explicit ByteColor_sRGB(const Color& c) noexcept {
        r = map_to<value_type>(linear_to_sRGB(std::clamp(c.r, 0.0f, 1.0f)));
        g = map_to<value_type>(linear_to_sRGB(std::clamp(c.g, 0.0f, 1.0f)));
        b = map_to<value_type>(linear_to_sRGB(std::clamp(c.b, 0.0f, 1.0f)));
        assert(r <= MAX && g <= MAX && b <= MAX);
        assert(r >= 0 && g >= 0 && b >= 0);
    };
};

constexpr ByteColor_sRGB to_ByteColor_sRGB(const Color& col) noexcept {
    return ByteColor_sRGB(col);
}

constexpr Color hadamard_product(const Color& a, const Color& b) noexcept {
    return a * b;
}

constexpr Color lerp(Color start, Color end, Real t) {    
    //return (start*(1.0f - t)) + (end*t);
    return start + (end - start) * t;
}

static constexpr Color BLACK = color(0, 0, 0);
static constexpr Color WHITE = color(1, 1, 1);
static constexpr Color RED = color(1, 0, 0);
static constexpr Color GREEN = color(0, 1, 0);
static constexpr Color BLUE = color(0, 0, 1);

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