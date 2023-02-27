#pragma once
#include "pch.h"
#include "Lights.h"
#include "Shapes.h"

struct World final {
    static constexpr auto DEFAULT_WORLD_MATERIAL = material(color(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);   
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
    Light light = point_light(point(-10, 10, -10), WHITE);    
    
    constexpr World() {                                      
        objects.push_back(sphere(DEFAULT_WORLD_MATERIAL));
        objects.push_back(sphere());
        std::get<Sphere>(objects.back()).transform = scaling(0.5f, 0.5f, 0.5f);
    }
    explicit constexpr World(std::initializer_list<value_type> list) {        
        objects.append_range(list);        
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
};

constexpr Sphere& get_sphere(World& w, size_t i) noexcept{    
    return std::get<Sphere>(w[i]);
}
constexpr const Sphere& get_sphere(const World& w, size_t i) noexcept{   
    return std::get<Sphere>(w[i]);
}