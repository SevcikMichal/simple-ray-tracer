#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "object.h"
#include "light.h"

class Scene {
public:
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<PointLight> pointLights;
    DirectionalLight dirLight;

    Scene(const DirectionalLight& dirLight) : dirLight(dirLight) {}

    // Add an object using move semantics
    void addObject(std::unique_ptr<Object> obj) { objects.push_back(std::move(obj)); }

    void addPointLight(const PointLight& l) { pointLights.push_back(l); }
};

#endif