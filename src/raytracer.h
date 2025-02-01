#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "ray.h"

class RayTracer {
public:
    static Vec3 trace(const Ray& r, const Scene& scene) {
        float closest_t = INFINITY;
        Vec3 hit_normal;
        const Object* hitObject = nullptr;

        for (const auto* object : scene.objects) {
            float t;
            Vec3 normal;
            if (object->hit(r, t, normal) && t < closest_t) {
                closest_t = t;
                hitObject = object;
                hit_normal = normal;
            }
        }

        if (hitObject) {
            Vec3 hit_point = r.at(closest_t);

            // Lighting calculation
            Vec3 light_color(0, 0, 0);
            
            for (const auto& light : scene.pointLights) {
                Vec3 light_dir = (light.position - hit_point).normalize();
                float diffuse = std::max(0.0f, hit_normal.dot(light_dir));
                light_color = light_color + (diffuse * light.color * light.intensity);
            }

            Vec3 dir_light_dir = scene.dirLight.direction;
            float diffuse_dir = std::max(0.0f, hit_normal.dot(dir_light_dir));
            Vec3 dir_light_contribution = diffuse_dir * scene.dirLight.color;

            return (light_color + dir_light_contribution) * hitObject->color;
        }

        // Background gradient
        Vec3 unit_direction = r.direction.normalize();
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
    }
};

#endif
