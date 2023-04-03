#pragma once
#include "pch.h"
#include "Tuple.h"
#include "Color.h"
#include "Matrix.h"

struct AlignCheck final {
    Color main;
    Color upper_left;
    Color upper_right;
    Color bottom_left;
    Color bottom_right;
};
constexpr AlignCheck uv_align_check(Color main, Color upper_left, Color upper_right,
                                                Color bottom_left, Color bottom_right) noexcept {
    return AlignCheck{ main, upper_left, upper_right, bottom_left, bottom_right };
}

constexpr Color uv_pattern_at(const AlignCheck& pattern, const UVCoords& uv) noexcept {
    if (uv.v > 0.8f) {
        if (uv.u < 0.2f) { return pattern.upper_left; }
        if (uv.u > 0.8f) { return pattern.upper_right; }
    }
    else if (uv.v < 0.2f) {
        if (uv.u < 0.2f) { return pattern.bottom_left; }
        if (uv.u > 0.8f) { return pattern.bottom_right;}
    }
    return pattern.main;
}

struct NullPattern final {
    constexpr Color at([[maybe_unused]] const Point& p) const noexcept { return MAGENTA; }
    explicit constexpr operator bool() const noexcept { return false; }
    constexpr bool operator==(const NullPattern& that) const noexcept = default;
    constexpr const Matrix4& get_transform() const noexcept { return _transform; }
    constexpr const Matrix4& inv_transform() const noexcept { return _invTransform; }
    constexpr void set_transform([[maybe_unused]] Matrix4 mat) noexcept {}
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
};

struct TestPattern final {
    constexpr Color at([[maybe_unused]] const Point& p) const noexcept { return color(p.x, p.y, p.z); }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==(const TestPattern& that) const noexcept = default;
    constexpr const Matrix4& get_transform() const noexcept { return _transform; }
    constexpr const Matrix4& inv_transform() const noexcept { return _invTransform; }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
};

struct StripePattern final {
    constexpr StripePattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{ b_ } {
        set_transform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {
        return (math::int_floor(p.x) % 2 == 0) ? a : b;
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==(const StripePattern& that) const noexcept = default;
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};

struct GradientPattern final {
    constexpr GradientPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{ b_ } {
        set_transform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {
        const auto dx = p.x - math::floor(p.x);
        return lerp(a, b, dx);
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==(const GradientPattern& that) const noexcept = default;
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};
struct RadialGradientPattern final {
    constexpr RadialGradientPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{ b_ } {
        set_transform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {
        const auto dist_from_center = magnitude(vector(p.x, p.y, p.z));
        const auto fraction = dist_from_center - math::floor(dist_from_center);
        return lerp(a, b, fraction);
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==(const RadialGradientPattern& that) const noexcept = default;
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};
struct RingPattern final {
    constexpr RingPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{ b_ } {
        set_transform(std::move(mat));
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr Color at(const Point& p) const noexcept {
        const auto distance_from_center = math::sqrt((p.x * p.x) + (p.z * p.z));
        const auto mod = math::int_floor(distance_from_center) % 2;
        return mod == 0 ? a : b;
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==(const RingPattern& that) const noexcept = default;
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};
struct CheckersPattern final {
    constexpr CheckersPattern(Matrix4 mat, Color a_, Color b_) noexcept : a{ a_ }, b{ b_ } {
        set_transform(std::move(mat));
    }
    constexpr Color at(const Point& p) const noexcept {
        const auto val = static_cast<int>(math::floor(p.x) + math::floor(p.y) + math::floor(p.z));
        return (val % 2 == 0) ? a : b;
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==(const CheckersPattern& that) const noexcept = default;
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Color a{};
    Color b{};
};
struct UVCheckers final {
    constexpr UVCheckers(unsigned width_, unsigned height_, Color a_, Color b_) noexcept
        : width{ width_ }, height{ height_ }, a{ a_ }, b{ b_ } {
    }
    constexpr Color at(const UVCoords& uv) const noexcept {
        const auto u2 = math::int_floor(uv.u * width);
        const auto v2 = math::int_floor(uv.v * height);
        const auto sum = u2 + v2;
        return (sum % 2 == 0) ? a : b;
    }
private:
    Color a{};
    Color b{};
    unsigned width = 0;
    unsigned height = 0;
};

struct TextureMap final {
    using Texture = UVCheckers;
    using Callable = std::function<UVCoords(const Point&)>;
    /*constexpr*/ TextureMap(Texture uv_pattern, Callable uv_map) noexcept
        : uv_pattern{ std::move(uv_pattern) }, uv_map{ std::move(uv_map) }
    {
    }
    /*constexpr*/ Color at(const Point& p) const noexcept {
        const auto texCoords = uv_map(p);
        return uv_pattern.at(texCoords);
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==([[maybe_unused]] const TextureMap& that) const noexcept {
        return true; /* TODO: the implementation is necessary for the build to succeed,
            as = default won't do. std::function isn't (easily) comparable anyway so some
            thought is needed to decide on how this comparison should operate.
            Right now all TextureMaps are functionally identical so returning true is mostly correct?
            The proper solution might be to move texture and uv mapping functionality away from the
            "Pattern"-structure */
    }
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    Texture uv_pattern;
    Callable uv_map;
};

struct CubeMap final {    
    /*constexpr*/ CubeMap(std::vector<AlignCheck*> patterns) noexcept
        : faces{ std::move(patterns) } 
    {
        assert(faces.size() == static_cast<int>(CubeFace::count) && "A CubeMap must have 6 patterns, one for each face.");
    }
    /*constexpr*/ Color at(const Point& p) const noexcept {
        const auto face = face_from_point(p);
        auto uvCoords = uv(0, 0);
        if (face == CubeFace::left) {
            uvCoords = cube_uv_left(p);
        }
        else if (face == CubeFace::right) {
            uvCoords = cube_uv_right(p);
        }
        else if (face == CubeFace::front) {
            uvCoords = cube_uv_front(p);
        }
        else if (face == CubeFace::back) {
            uvCoords = cube_uv_back(p);
        }
        else if (face == CubeFace::up) {
            uvCoords = cube_uv_up(p);
        }
        else { //CubeFace::down
            uvCoords = cube_uv_down(p);
        }
        const auto i = std::to_underlying(face);
        assert(i > 0 && i < static_cast<int>(CubeFace::count));
        return uv_pattern_at(*faces[i], uvCoords);        
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    explicit constexpr operator bool() const noexcept { return true; }
    constexpr bool operator==([[maybe_unused]] const CubeMap& that) const noexcept {
        return true; 
    }
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
    std::vector<AlignCheck*> faces;    
};

/*constexpr*/ auto cube_map(std::vector<AlignCheck*> patterns) noexcept {
    assert(patterns.size() == static_cast<int>(CubeFace::count) && "A CubeMap must have 6 patterns, one for each face.");
    return CubeMap(std::move(patterns));
}

/*constexpr*/ auto texture_map(TextureMap::Texture uv_pattern, TextureMap::Callable uv_map) noexcept {
    return TextureMap(std::move(uv_pattern), std::move(uv_map));
}

constexpr auto null_pattern() noexcept {
    return NullPattern{};
};
constexpr auto test_pattern() noexcept {
    return TestPattern();
};

constexpr auto stripe_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return StripePattern(std::move(m), a, b);
};
constexpr auto gradient_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return GradientPattern(std::move(m), a, b);
};
constexpr auto radial_gradient_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return RadialGradientPattern(std::move(m), a, b);
};
constexpr auto ring_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return RingPattern(std::move(m), a, b);
};
constexpr auto checkers_pattern(Color a, Color b, Matrix4 m = Matrix4Identity) noexcept {
    return CheckersPattern(std::move(m), a, b);
};
constexpr auto uv_checkers(unsigned width, unsigned height, Color a, Color b) noexcept {
    return UVCheckers(width, height, a, b);
};
using Patterns = std::variant<NullPattern, TestPattern, StripePattern, GradientPattern, RingPattern, CheckersPattern, RadialGradientPattern, TextureMap>;

template<typename T>
concept is_pattern = std::is_same_v<NullPattern, T> || std::is_same_v<TestPattern, T> ||
std::is_same_v<StripePattern, T> || std::is_same_v<GradientPattern, T> ||
std::is_same_v<RingPattern, T> || std::is_same_v<CheckersPattern, T> ||
std::is_same_v<RadialGradientPattern, T> || std::is_same_v<TextureMap, T>;

template<typename T>
    requires is_pattern<T>
inline bool operator==(const T& t, const Patterns& v) {
    const T* c = std::get_if<T>(&v);
    return c && *c == t; // true if v contains a pattern that compares equal to v
}

inline bool operator==(const Patterns& v, const is_pattern auto& t) {
    return t == v;
};

constexpr void set_transform(Patterns& variant, const Matrix4& newTransform) noexcept {
    return std::visit([&newTransform](auto& pattern) noexcept -> void {
        return pattern.set_transform(newTransform);
        }, variant);
};
constexpr const Matrix4& get_transform(const Patterns& variant) noexcept {
    return std::visit([](const auto& pattern) noexcept -> const Matrix4& {
        return pattern.get_transform();
        }, variant);
};
constexpr const Matrix4& get_inverse_transform(const Patterns& variant) noexcept {
    return std::visit([](const auto& pattern) noexcept -> const Matrix4& {
        return pattern.inv_transform();
        }, variant);
};

Color pattern_at(const Patterns& variant, const Point& p) noexcept {
    assert(!std::holds_alternative<NullPattern>(variant) && "pattern_at: called on NullPattern.");
    return std::visit([&p](const auto& pattern) noexcept -> Color { return pattern.at(p); }, variant);
};

//#include "Shapes.h"
//template<typename T>
//requires is_shape<T>
Color pattern_at(const Patterns& pattern, const /*must be is_shapes but I can't name that here. got some circular dependency going on.*/ auto& obj, const Point& world_point) noexcept {
    assert(!std::holds_alternative<NullPattern>(pattern) && "pattern_at: called on NullPattern.");
    const auto object_point = get_inverse_transform(obj) * world_point;
    const auto pattern_point = get_inverse_transform(pattern) * object_point;
    return pattern_at(pattern, pattern_point);
};

/*constexpr*/ UVCoords spherical_map(const Point& p) noexcept {
    // Compute the azimuthal angle
    // -π < theta <= π
    // Angle increases clockwise as viewed from above,
    // which is opposite of what we want, but we'll fix it later.
    const auto theta = std::atan2(p.x, p.z);

    // Compute the polar angle
    // 0 <= phi <= π
    //we are treating the point as a vector pointing from the sphere's origin (the world origin)
    //to the point, and calculating the magnitude of that get the sphere's radius.
    const auto phi = std::acos(p.y / math::sqrt(math::square(p.x) + math::square(p.y) + math::square(p.z)));

    // -0.5 < raw_u <= 0.5
    const auto raw_u = theta / math::TWO_PI;

    // 0 <= u < 1
    // Here's also where we fix the direction of u. Subtract it from 1,
    // so that it increases counterclockwise as viewed from above.
    const auto u = 1.0f - (raw_u + 0.5f);

    // We want v to be 0 at the south pole of the sphere,
    // and 1 at the north pole, so we have to "flip it over"
    // by subtracting it from 1.
    const auto v = 1 - phi / math::PI;

    return { u, v };
}

/*planar mapping tiles every unit square on the plane, and ignores the y coordinate.*/
/*constexpr*/ UVCoords planar_map(const Point& p) noexcept {
    const auto u = p.x - std::floor(p.x); // treat the fractional portion of the x coordinate as u
    const auto v = p.z - std::floor(p.z); //and the fractional portion of z as v.
    return uv(u, v);
}

/*planar mapping tiles every unit square on the plane, and ignores the y coordinate.*/
/*constexpr*/ UVCoords cylindrical_map(const Point& p) noexcept {
    //compute the azimuthal angle, same as with spherical_map()
    const auto theta = std::atan2(p.x, p.z);
    const auto raw_u = theta / math::TWO_PI;
    const auto u = 1 - (raw_u + 0.5f);
    // let v go from 0 to 1 between whole units of y
    const auto v = p.y - std::floor(p.y);
    return uv(u, v);
}