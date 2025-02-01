#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "ray.h"
#include <random>

class RayTracer {
public:
    static Vec3 trace(const Ray& r, const Scene& scene) {
        float closest_t = INFINITY;
        Vec3 hit_normal;
        const Object* hitObject = nullptr;

        // Find the closest object hit by the ray
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
            Vec3 total_light(0, 0, 0);

            // Process each light in the scene
            for (const auto& light : scene.pointLights) {
                total_light = total_light + computeSoftShadow(hit_point, hit_normal, light, scene);
            }

            // Directional light contribution
            Vec3 dir_light_dir = scene.dirLight.direction;
            float diffuse_dir = std::max(0.0f, hit_normal.dot(dir_light_dir));
            Vec3 dir_light_contribution = diffuse_dir * scene.dirLight.color;

            return (total_light + dir_light_contribution) * hitObject->color;
        }

        // Background gradient
        Vec3 unit_direction = r.direction.normalize();
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
    }

    static Vec3 computeSoftShadow(const Vec3& hit_point, const Vec3& normal, const PointLight& light, const Scene& scene) {
        int shadow_samples = 16;  // Increase for smoother shadows
        int visible_samples = 0;
    
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> rand_offset(-light.radius, light.radius);
    
        for (int i = 0; i < shadow_samples; i++) {
            Vec3 jittered_light_pos = light.position + Vec3(rand_offset(gen), rand_offset(gen), rand_offset(gen));
            Vec3 light_dir = (jittered_light_pos - hit_point).normalize();
            Ray shadow_ray(hit_point + normal * 0.001f, light_dir);  // Offset to avoid self-shadowing
    
            bool in_shadow = false;
            for (const auto* object : scene.objects) {
                float t;
                Vec3 temp_normal;
                if (object->hit(shadow_ray, t, temp_normal)) {
                    in_shadow = true;
                    break;
                }
            }
    
            if (!in_shadow) {
                visible_samples++;
            }
        }
    
        // Light visibility factor
        float visibility = float(visible_samples) / float(shadow_samples);
        float diffuse = std::max(0.0f, normal.dot((light.position - hit_point).normalize()));
        return visibility * diffuse * light.color * light.intensity;
    }    
};

#endif
