#pragma once
#include "pch.h"
#include "Math.h"
#include "Matrix.h"
#include "Ray.h"
#include "Lights.h"
#include "Lighting.h"
#include "World.h"
#include "Canvas.h"
#include "WorkQue.h"

struct Camera final {
    using size_type = Canvas::size_type;
    Real field_of_view = math::PI;
    Real half_width = 0;
    Real half_height = 0;
    Real pixel_size = 0;
    size_type width = 160; //horizontal size, in pixels
    size_type height = 120; //vertical size, in pixels

    /*constexpr*/ Camera(size_type hsize_, size_type vsize_, Real field_of_view_) noexcept
        : field_of_view(field_of_view_), width(hsize_), height(vsize_) {
        const auto half_view = std::tan(field_of_view / 2.0f); //TODO: constexpr tan
        auto widthf = static_cast<float>(width);
        if (const Real aspect = widthf / static_cast<float>(height); aspect >= 1) {
            half_width = half_view;
            half_height = half_view / aspect;
        }
        else {
            half_width = half_view * aspect;
            half_height = half_view;
        }
        pixel_size = half_width * 2.0f / widthf;
    }
    /*constexpr*/ Camera(size_type hsize_, size_type vsize_, Real field_of_view_, Matrix4 viewTransform) noexcept
        : Camera(hsize_, vsize_, field_of_view_) {
        set_transform(viewTransform);
    }
    constexpr const Matrix4& get_transform() const noexcept {
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept {
        return _invTransform;
    }
    constexpr void set_transform(Matrix4 mat) noexcept {
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
private:
    Matrix4 _transform{ Matrix4Identity };
    Matrix4 _invTransform{ Matrix4Identity };
};

struct OrthographicCamera final {
    using size_type = Canvas::size_type;
    size_type width = 160;
    size_type height = 120;
    Point position;
    Vector direction;
    Vector up;
    Vector right;

    constexpr OrthographicCamera(size_type hsize_, size_type vsize_,
        Point pos, Vector dir, Vector up, Vector right) noexcept
        : width(hsize_), height(vsize_), position(pos), direction(normalize(dir)), up(up), right(right)
    {}
};

constexpr Ray ray_for_pixel(const OrthographicCamera& c, size_t px, size_t py) noexcept {
    const auto dx = px / static_cast<Real>(c.width);
    const auto dy = 1 - py / static_cast<Real>(c.height);
    const auto right = c.right * dx;
    const auto up = c.up * dy;
    const auto origin = c.position + right + up;    
    return ray(origin, c.direction);
}

constexpr Ray ray_for_pixel(const Camera& c, size_t px, size_t py) noexcept {
    //offset from edge of the canvas to the pixel's center
    const auto xoffset = float(px + 0.5f) * c.pixel_size;
    const auto yoffset = float(py + 0.5f) * c.pixel_size;
    //untransformed coordinates of the pixel in world space
    //remember that the camera looks towards -z, so +x is to the *left*
    const auto world_x = c.half_width - xoffset;
    const auto world_y = c.half_height - yoffset;
    //using the camera matrix, transform the canvas point and the origin, 
    //and then compute the ray's direction. (canvas is at z = -1)
    const auto pixel = c.inv_transform() * point(world_x, world_y, -1.0f);
    auto origin = c.inv_transform() * ORIGO;
    auto direction = normalize(pixel - origin);
    return ray(origin, direction);
}

constexpr Canvas render_single_threaded(const Camera& camera, const World& w) {
    using size_type = Canvas::size_type;
    Canvas img(camera.width, camera.height);
    for (size_type y = 0; y < img.height(); ++y) {
        for (size_type x = 0; x < img.width(); ++x) {
            const auto color = color_at(w, ray_for_pixel(camera, x, y));
            img.set(x, y, color);
        }
    }
    return img;
}

Canvas render_multi_threaded(const Camera& camera, const World& world) {
    using size_type = Canvas::size_type;
    Canvas canvas(camera.width, camera.height);
    WorkQue worker;
    worker.schedule(canvas.size(), [&world, &camera, &canvas, width = canvas.width(), height = canvas.height()]([[maybe_unused]] size_t part, size_t i) noexcept {
        const auto x = index_to_column(i, width);
        const auto y = index_to_row(i, height);
        canvas[i] = color_at(world, ray_for_pixel(camera, x, y));
        });
    worker.run_in_parallel();
    return canvas;
}

Canvas render(const Camera& camera, const World& world) {
    if constexpr (RUN_SEQUENTIAL) {
        return render_single_threaded(camera, world);
    }
    else {
        return render_multi_threaded(camera, world);
    }
}