#pragma once
#include "pch.h"
#include "Matrix.h"
#include "Shapes.h"

struct Group final{
    constexpr bool empty() const noexcept{ return _shapes.empty(); }
    constexpr size_t size() const noexcept{ return _shapes.size(); }

    constexpr auto operator==(const Group& that) const noexcept{
        return _shapes == that._shapes && _transform == that._transform;
    }
    constexpr const Matrix4& get_transform() const noexcept{
        return _transform;
    }
    constexpr const Matrix4& inv_transform() const noexcept{
        return _invTransform;
    }
    constexpr void set_transform(Matrix4 mat) noexcept{
        _transform = std::move(mat);
        _invTransform = inverse(_transform);
    }
    constexpr const Material& surface() const noexcept{
        return _surface;
    }
    constexpr const Color& color() const noexcept{
        return surface().color;
    }
    constexpr Material& surface() noexcept{
        return _surface;
    }
    constexpr Color& color() noexcept{
        return surface().color;
    }
    constexpr bool has_parent() const noexcept{
        return _parent != nullptr;
    }
    constexpr auto& front() noexcept{
        return *_shapes.front();
    }
    constexpr auto& back() noexcept{
        return *_shapes.back();
    }
    constexpr auto begin() noexcept{
        return _shapes.begin();
    }
    constexpr auto end() noexcept{
        return _shapes.end();
    }
    constexpr void push_back(Shapes* s){
        _shapes.push_back(s);
        ::set_parent(*s, this);
    }
    constexpr void set_parent(Group* g) noexcept{
        _parent = g;
    }
    constexpr Group* get_parent() const noexcept{
        return _parent;
    }
private:
    Material _surface{material()};
    Matrix4 _transform{Matrix4Identity};
    Matrix4 _invTransform{Matrix4Identity};
    std::vector<Shapes*> _shapes;
    Group* _parent = nullptr;
};

constexpr Vector local_normal_at([[maybe_unused]] const Group& g, const Point& object_space_point) noexcept{
    assert(false && "groups don't have normals. only shapes do!");
    return normalize(vector(object_space_point)); /*imagine object_space_point - s.position, but s position is always 0*/
}

constexpr auto local_intersect([[maybe_unused]] const Group& g, [[maybe_unused]] const Ray& local_ray) noexcept{
    return std::vector<Real>();
}