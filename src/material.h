#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:
    Vec3 diffuse;
    Vec3 specular;
    float shininess;
    float reflectivity;
    float ior;  // Index of Refraction

    Material(const Vec3& d, const Vec3& s, float sh, float refl = 0.0f, float ior = 1.0f)
        : diffuse(d), specular(s), shininess(sh), reflectivity(refl), ior(ior) {}
};

#endif
