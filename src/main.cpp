#include <iostream>
#include <fstream>
#include "ray.h"
#include "light.h"

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

Vec3 ray_color(const Ray& r, const PointLight& point_light, const Vec3& dir_light_dir, const Vec3& dir_light_color) {
    float t = hit_sphere(Vec3(0, 0, -1), 0.5, r);
    if (t > 0.0f) {
        Vec3 hit_point = r.origin + r.direction * t;
        Vec3 normal = (hit_point - Vec3(0, 0, -1)).normalize();

        // **Directional Light**
        Vec3 dir_light = dir_light_dir.normalize();
        float diffuse_dir = std::max(0.0f, normal.dot(dir_light));
        float specular_dir = pow(std::max(0.0f, normal.dot((dir_light + (-r.direction.normalize())).normalize())), 128);
        Vec3 dir_light_contribution = (diffuse_dir + specular_dir) * dir_light_color;

        // **Point Light**
        Vec3 light_dir = (point_light.position - hit_point).normalize();
        Vec3 view_dir = -r.direction.normalize();
        Vec3 halfway = (light_dir + view_dir).normalize();
        float diffuse_point = std::max(0.0f, normal.dot(light_dir));
        float specular_point = pow(std::max(0.0f, normal.dot(halfway)), 128);
        Vec3 point_light_contribution = (diffuse_point + specular_point) * point_light.color * point_light.intensity;

        // **Final Lighting**
        Vec3 base_color = Vec3(1, 1, 1);  // Red Sphere
        Vec3 final_color = (dir_light_contribution + point_light_contribution) * base_color;

        // Clamp colors to prevent overflow
        return Vec3(
            std::min(1.0f, final_color.x),
            std::min(1.0f, final_color.y),
            std::min(1.0f, final_color.z)
        );
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

    // **Define Lights**
    PointLight point_light(Vec3(2, 1, 1), Vec3(0, 1, 1), 0.5f);  // White Point Light
    Vec3 dir_light_dir = Vec3(-1, 1, 1).normalize();  // Directional Light from top left
    Vec3 dir_light_color = Vec3(0.8, 0.8, 0.8);  // Slightly dim white directional light

    std::ofstream image("output.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / width;
            float v = float(j) / height;
            Ray r(origin, lower_left + u * horizontal + v * vertical);
            Vec3 col = ray_color(r, point_light, dir_light_dir, dir_light_color);

            int ir = int(255.00 * std::min(1.0f, col.x));
            int ig = int(255.00 * std::min(1.0f, col.y));
            int ib = int(255.00 * std::min(1.0f, col.z));
            image << ir << " " << ig << " " << ib << "\n";
        }
    }

    image.close();
    std::cout << "Image rendered: output.ppm\n";
}