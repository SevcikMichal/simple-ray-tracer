#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:
    Vec3 diffuse;   // Diffuse color
    Vec3 specular;  // Specular color
    float shininess; // Shininess exponent

    Material(const Vec3& d, const Vec3& s, float sh)
        : diffuse(d), specular(s), shininess(sh) {}
};

#endif