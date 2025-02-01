#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float t) const { return Vec3(x * t, y * t, z * t); }
    Vec3 operator/(float t) const { return Vec3(x / t, y / t, z / t); }

    // Ensure scalar multiplication works both ways
    friend Vec3 operator*(float t, const Vec3& v) {
        return Vec3(t * v.x, t * v.y, t * v.z);
    }

    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

    Vec3 normalize() const {
        float length = std::sqrt(dot(*this));
        return *this / length;
    }
};

#endif
