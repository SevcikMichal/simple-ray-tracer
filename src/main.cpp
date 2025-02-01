#include <iostream>
#include <fstream>
#include "ray.h"

float hit_sphere(const Vec3& center, float radius, const Ray& r) {
    Vec3 oc = r.origin - center;
    float a = r.direction.dot(r.direction);
    float b = 2.0f * oc.dot(r.direction);
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0f;  // No intersection
    } else {
        return (-b - sqrt(discriminant)) / (2.0f * a);  // Closest intersection
    }
}

Vec3 ray_color(const Ray& r) {
    float t = hit_sphere(Vec3(0, 0, -1), 0.5, r);
    if (t > 0.0f) {  // The ray hit the sphere
        Vec3 hit_point = r.origin + r.direction * t;
        Vec3 normal = (hit_point - Vec3(0, 0, -1)).normalize();

        Vec3 light_dir = Vec3(-0.5, 1, 1).normalize();  // Light from top-left front
        Vec3 view_dir = -r.direction.normalize();  // Camera looks in -ray direction
        Vec3 halfway = (light_dir + view_dir).normalize();  // Blinn-Phong halfway vector

        float diffuse = std::max(0.0f, normal.dot(light_dir));  // Diffuse shading
        float specular = pow(std::max(0.0f, normal.dot(halfway)), 128);  // Shininess

        Vec3 base_color = Vec3(1, 0, 0);  // Red base color
        Vec3 final_color = diffuse * base_color + specular * Vec3(1, 1, 1);  // White specular

        return final_color;
    }

    // Background gradient
    Vec3 unit_direction = r.direction.normalize();
    t = 0.5f * (unit_direction.y + 1.0f);
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
