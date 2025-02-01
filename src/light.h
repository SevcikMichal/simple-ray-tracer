#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"

class PointLight {
public:
    Vec3 position;
    Vec3 color;
    float intensity;
    float radius;  // Defines how soft the shadows are

    PointLight(const Vec3& pos, const Vec3& col, float intensity = 1.0f, float radius = 0.2f)
        : position(pos), color(col), intensity(intensity), radius(radius) {}
};

class DirectionalLight {
public:
    Vec3 direction;
    Vec3 color;
    float radius;

    DirectionalLight(const Vec3& dir, const Vec3& col, float radius = 0.1f) 
        : direction(dir.normalize()), color(col), radius(radius) {}
};

#endif
