#pragma once
#include "pch.h"
#include "Math.h"
#include "Matrix.h"
#include "Ray.h"
#include "World.h"
#include "Canvas.h"

struct Camera final{
    Matrix4 transform = Matrix4Identity;        
    Real hsize = 160; 
    Real vsize = 120; 
    Real field_of_view = math::PI;
    Real half_width = 0; 
    Real half_height = 0;
    Real pixel_size = 0; 

    /*constexpr*/ Camera(Real hsize_, Real vsize_, Real field_of_view_) noexcept 
        : hsize(hsize_), vsize(vsize_), field_of_view(field_of_view_) {
        const Real half_view = std::tan(field_of_view / 2.0f); //TODO: constexpr tan
        if(const Real aspect = hsize / vsize; aspect >= 1){
            half_width = half_view;
            half_height = half_view / aspect;
        }else{
            half_width = half_view * aspect;
            half_height = half_view;
        }
        pixel_size = (half_width * 2) / hsize;
    }
};

constexpr Ray ray_for_pixel(const Camera& c, Real px, Real py) noexcept{
    //offset from edge of the canvas to the pixel's center
    const auto xoffset = (px + 0.5f) * c.pixel_size;
    const auto yoffset = (py + 0.5f) * c.pixel_size;
    //untransformed coordinates of the pixel in world space
    //remember that the camera looks towards -z, so +x is to the *left*
    const auto world_x = c.half_width - xoffset;
    const auto world_y = c.half_height - yoffset;
    //using the camera matrix, transform the canvas point and the origin, 
    //and then compute the ray's direction. (canvas is at z = -1)
    const auto inv = inverse(c.transform);
    const auto pixel = inv * point(world_x, world_y, -1.0f);
    const auto origin = inv * point(0,0,0);
    const auto direction = normalize(pixel - origin);
    return ray(origin, direction);
}

constexpr Canvas render(const Camera& camera, const World& w) {
    using size_type = Canvas::size_type;
    Canvas img(narrow_cast<size_type>(camera.hsize), narrow_cast<size_type>(camera.vsize));
    for(size_type y = 0; y < img.height(); ++y){
        for (size_type x = 0; x < img.width(); ++x) {
            const auto color = color_at(w, ray_for_pixel(camera, x, y));
            img.set(x, y, color);
        }
    }
    return img;
}