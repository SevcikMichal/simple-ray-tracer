#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"

class Object {
public:
    Vec3 color;

    Object(const Vec3& col) : color(col) {}

    virtual ~Object() {}

    // Every object must implement its own `hit` function
    virtual bool hit(const Ray& r, float& t, Vec3& normal) const = 0;
};

#endif
