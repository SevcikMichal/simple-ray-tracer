#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "light.h"

class Scene {
public:
    std::vector<Object*> objects;
    std::vector<PointLight> pointLights;
    DirectionalLight dirLight;

    Scene(const DirectionalLight& dirLight) : dirLight(dirLight) {}

    void addObject(Object* obj) { objects.push_back(obj); }
    void addPointLight(const PointLight& l) { pointLights.push_back(l); }
};

#endif
