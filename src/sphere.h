#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object {
public:
    Vec3 center;
    float radius;

    Sphere(const Vec3& c, float r, const Vec3& col) : Object(col), center(c), radius(r) {}

    bool hit(const Ray& r, float& t, Vec3& normal) const override {
        Vec3 oc = r.origin - center;
        float a = r.direction.dot(r.direction);
        float b = 2.0f * oc.dot(r.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;

        t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        if (t < 0) return false;

        normal = (r.at(t) - center).normalize();
        return true;
    }
};

#endif
