#pragma once
#include "pch.h"
#include "Matrix.h"
#include "Color.h"
#include "Material.h"
#include "Shapes_fwd.h"

struct Ray;

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
     constexpr auto begin() const noexcept{
        return _shapes.cbegin();
    }
    constexpr auto end() const noexcept{
        return _shapes.cend();
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

// Declaration only; the definition appears in shapes.h.
//
// Reason:
// Group is itself a “shape” and contains other Shapes. At the same time,
// individual Shapes need to know their parent Group. This creates a recursive
// relationship between Group and Shapes that cannot be resolved in a single
// header.
//
// By declaring local_intersect(Group, Ray) here, Group can expose the function
// without needing the full variant-based dispatch logic. The actual definition
// is provided in shapes.h, once both Group and the Shapes variant are fully
// known.
constexpr std::vector<Real> local_intersect(const Group& group, const Ray& local_ray) noexcept;
