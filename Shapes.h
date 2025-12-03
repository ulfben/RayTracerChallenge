#pragma once
#include "pch.h"
#include "Shapes_fwd.h" //gets Shapes and is_shape
#include "Group.h"
#include "Ray.h"
#include "Matrix.h"
#include "Color.h"
#include "Material.h"

constexpr Vector normal_at(const Shapes& variant, const Point& p){
    return std::visit([&p](const auto& obj) noexcept {
      if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            // Assuming Group has the necessary methods (this part may need adjustments)
            const auto object_space_point = obj->inv_transform() * p; 
            const auto object_space_normal = local_normal_at(*obj, object_space_point);
            auto world_space_normal = transpose(obj->inv_transform()) * object_space_normal;        
            return normalize(world_space_normal);
        } else {
            // Handle other shapes
            const auto object_space_point = obj.inv_transform() * p; 
            const auto object_space_normal = local_normal_at(obj, object_space_point);
            auto world_space_normal = transpose(obj.inv_transform()) * object_space_normal;        
            return normalize(world_space_normal);    
        }        
        }, variant);
}
template<typename T>
    requires is_shape<T>
constexpr bool operator==(const T& t, const Shapes& v){
    const T* c = std::get_if<T>(&v);
    return c && *c == t; // true if v contains a pattern that compares equal to v
}

constexpr bool operator==(const Patterns& v, const is_shape auto& t){
    return t == v;
};

//functions handling the Shapes variant
constexpr const Matrix4& get_transform(const Shapes& variant) noexcept {
    return std::visit([](const auto& obj) -> const Matrix4& {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            return obj->get_transform();
        } else {
            return obj.get_transform();
        }
    }, variant);
}
constexpr const Matrix4& get_inverse_transform(const Shapes& variant) noexcept {
    return std::visit([](const auto& obj) -> const Matrix4& {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            return obj->inv_transform();
        } else {
            return obj.inv_transform();
        }
    }, variant);
}
constexpr const Material& surface(const Shapes& variant) noexcept {
    return std::visit([](const auto& obj) constexpr noexcept -> const Material& {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            return obj->surface();
        } else {
            return obj.surface();
        }
    }, variant);
}
constexpr Material& surface(Shapes& variant) noexcept {
    return std::visit([](auto& obj) constexpr noexcept -> Material& {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            return obj->surface();
        } else {
            return obj.surface();
        }
    }, variant);
}

constexpr const Color& color(const Shapes& variant) noexcept {
    return std::visit([](const auto& obj) -> const Color& {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            return obj->color();
        } else {
            return obj.color();
        }
    }, variant);
}

constexpr void set_transform(Shapes& variant, const Matrix4& t) noexcept {
    return std::visit([t](auto& obj) noexcept -> void {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            obj->set_transform(t);
        } else {
            obj.set_transform(t);
        }
    }, variant);
}

inline constexpr void set_parent(Shapes& variant, Group* g) noexcept {
    return std::visit([g](auto& obj) noexcept -> void {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            obj->set_parent(g);
        } else {
            obj.set_parent(g);
        }
    }, variant);
}

constexpr Group* get_parent(Shapes& variant) noexcept {
    return std::visit([](const auto& obj) noexcept -> Group* {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            return obj->get_parent();
        } else {
            return obj.get_parent();
        }
    }, variant);
}

constexpr auto local_intersect(const Shapes& variant, const Ray& r) noexcept {
    return std::visit([r](const auto& obj) noexcept -> auto {
        if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Group*>) {
            assert(obj != nullptr && "Null Group pointer encountered");
            return ::local_intersect(*obj, r);
        } else {
            return ::local_intersect(obj, r);
        }
    }, variant);
}

//functions handling the individual geometry types
constexpr const Material& surface(const is_shape auto& obj) noexcept{
    return obj.surface();
}

constexpr const Matrix4& get_transform(const is_shape auto& obj) noexcept{
    return  obj.get_transform();
}

constexpr const Matrix4& get_inverse_transform(const is_shape auto& obj) noexcept{
    return obj.inv_transform();
}

constexpr const Color& color(const is_shape auto& obj) noexcept{
    return obj.color();
}

std::ostream& operator<<(std::ostream& os, const Shapes& variant){
    return std::visit([&os](const auto& val) -> std::ostream&{
        os << val;
        return os;
        }, variant);
}

inline constexpr std::vector<Real> local_intersect(const Group& group, const Ray& local_ray) noexcept{
    for(const auto shape : group){
        const Shapes temp = *shape;
        auto xs = local_intersect(temp, local_ray);

    }
    //TODO: implement
    return std::vector<Real>();
}