#pragma once
#include "pch.h"
#include "Matrix.h"
#include "Lights.h"
#include "Shapes.h"
#include "Material.h"

struct World final {
    static constexpr auto DEFAULT_MATERIAL = material(color(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);   
    static constexpr auto DEFAULT_LIGHT = point_light(point(-10, 10, -10), WHITE);  
    using value_type = Shapes;    
    using container = std::vector<value_type>;
    using reference = container::reference;
    using const_reference = container::const_reference;
    using pointer = container::pointer;
    using const_pointer = container::const_pointer;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;
    using size_type = container::size_type;
    
    container objects;    
    Light light = DEFAULT_LIGHT;    
    
    constexpr World() {                                      
        objects.emplace_back(sphere(DEFAULT_MATERIAL));
        objects.emplace_back(sphere(scaling(0.5f, 0.5f, 0.5f)));        
    }
    explicit constexpr World(Light l) : World() {                
        light = std::move(l);
    }
    explicit constexpr World(std::initializer_list<value_type> list) {        
        objects.append_range(list);        
    }
    explicit constexpr World(std::initializer_list<value_type> list, Light l) : World(list) {        
        light = std::move(l);
    }
    constexpr void push_back(std::initializer_list<value_type> list) {        
        objects.append_range(list);        
    }
    constexpr void push_back(value_type shape) {        
        objects.push_back(std::move(shape));
    }
    constexpr bool contains(const value_type& object) const noexcept {               
        return std::ranges::find(objects, object) != objects.end();        
    }   
    constexpr const_reference operator[](size_type i) const noexcept {
        assert(i < size() && "World::operator[i] index is out of bounds");
        return objects[i];
    }
    constexpr reference operator[](size_type i) noexcept {
        assert(i < size() && "World::operator[i] index is out of bounds");
        return objects[i];
    }  
    explicit constexpr operator bool() const noexcept {
        return !empty();
    }
    constexpr pointer data() noexcept { return objects.data(); }
    constexpr const_pointer data() const noexcept { return objects.data(); }
    constexpr size_type size() const noexcept { return objects.size(); }
    constexpr size_type count() const noexcept { return size(); }
    constexpr bool empty() const noexcept { return objects.empty(); }
    constexpr iterator begin() noexcept { return objects.begin(); }
    constexpr iterator end() noexcept { return objects.end(); }
    constexpr const_iterator begin() const noexcept { return objects.begin(); }
    constexpr const_iterator end() const noexcept { return objects.end(); }
    constexpr const_reference back() const noexcept {
        assert(!empty() && "World::back() on empty world is undefined behavior!");
        return objects[size()-1];
    }
    constexpr reference back() noexcept {
        assert(!empty() && "World::back() on empty world is undefined behavior!");
        return objects[size()-1];
    }  
};

constexpr Material& get_material(World& w, size_t i) noexcept{    
    return std::visit([](auto& obj) noexcept -> Material& {return obj.surface;  }, w[i]);
}
constexpr const Material& get_material(const World& w, size_t i) noexcept{   
    return std::visit([](const auto& obj) noexcept -> const Material& {return obj.surface;  }, w[i]);
}

constexpr const Matrix4& get_transform(const World& w, size_t i) noexcept{   
    return std::visit([](const auto& obj) noexcept -> const Matrix4& {return obj.transform();  }, w[i]);
}

constexpr Material& get_material(World::iterator iter) noexcept{    
    return std::visit([](auto& obj) noexcept -> Material& {return obj.surface;  }, *iter);
}
constexpr const Material& get_material(World::const_iterator iter) noexcept{   
    return std::visit([](const auto& obj) noexcept -> const Material& {return obj.surface;  }, *iter);
}

constexpr const Matrix4& get_transform(World::const_iterator iter) noexcept{   
    return std::visit([](const auto& obj) noexcept -> const Matrix4& {return obj.transform();  }, *iter);
}