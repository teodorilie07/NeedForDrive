#pragma once
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <cmath>
#include <type_traits>
#include <SFML/Graphics.hpp>
#include "erori.h"
#include "vector.h"

template <typename T>
class ResourceManager {
private:
    std::map<std::string, T> resources;
    ResourceManager() = default;

public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager<T>& getInstance() {
        static ResourceManager<T> instance;
        return instance;
    }

    T& get(const std::string& path) {
        auto it = resources.find(path);
        if (it != resources.end()) {
            return it->second;
        }

        T resource;
        bool success = false;
        if constexpr (std::is_same_v<T, sf::Font>) {
            success = resource.openFromFile(path);
        } else {
            success = resource.loadFromFile(path);
        }

        if (!success) {
            throw FileLoadException(path);
        }

        auto [newIt, inserted] = resources.insert({path, std::move(resource)});
        return newIt->second;
    }
};

// Template function to calculate distance between two objects that have getPozitie()
template <typename T, typename U>
float getDistance(const T& obj1, const U& obj2) {
    vector p1 = obj1.getPozitie();
    vector p2 = obj2.getPozitie();
    float dx = p1.getx() - p2.getx();
    float dy = p1.gety() - p2.gety();
    return std::sqrt(dx * dx + dy * dy);
}
