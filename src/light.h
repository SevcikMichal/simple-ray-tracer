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

#endif
