#include <iostream>
#include <fstream>
#include "ray.h"

bool hit_sphere(const Vec3& center, float radius, const Ray& r) {
    Vec3 oc = r.origin - center;
    float a = r.direction.dot(r.direction);
    float b = 2.0f * oc.dot(r.direction);
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

Vec3 ray_color(const Ray& r) {
    if (hit_sphere(Vec3(0, 0, -1), 0.5, r))
        return Vec3(1, 0, 0); // Red for hits

    Vec3 unit_direction = r.direction.normalize();
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main() {
    const int width = 800, height = 400;
    Vec3 lower_left(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);

    std::ofstream image("output.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / width;
            float v = float(j) / height;
            Ray r(origin, lower_left + u * horizontal + v * vertical);
            Vec3 col = ray_color(r);

            int ir = int(255.00 * col.x);
            int ig = int(255.00 * col.y);
            int ib = int(255.00 * col.z);
            image << ir << " " << ig << " " << ib << "\n";
        }
    }

    image.close();
    std::cout << "Image rendered: output.ppm\n";
}
