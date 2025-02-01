#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "material.h"

class Object {
public:
    Material material;

    Object(const Material& mat) : material(mat) {}
    virtual ~Object() {}

    // Every object must implement its own `hit` function
    virtual bool hit(const Ray& r, float& t, Vec3& normal) const = 0;
};

#endif
