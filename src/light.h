#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"

class PointLight {
public:
    Vec3 position;
    Vec3 color;
    float intensity;

    PointLight(const Vec3& pos, const Vec3& col, float intensity = 1.0f)
        : position(pos), color(col), intensity(intensity) {}
};

class DirectionalLight {
public:
    Vec3 direction;
    Vec3 color;

    DirectionalLight(const Vec3& dir, const Vec3& col) : direction(dir.normalize()), color(col) {}
};

#endif
