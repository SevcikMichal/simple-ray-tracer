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

        for (const auto& object : scene.objects) {
            float t;
            Vec3 normal;
            if (object->hit(r, t, normal) && t < closest_t) {
                closest_t = t;
                hitObject = object.get();
                hit_normal = normal;
            }
        }

        if (hitObject) {
            Vec3 hit_point = r.at(closest_t);
            Vec3 total_light(0, 0, 0);

            for (const auto& light : scene.pointLights) {
                total_light = total_light + computeLighting(hit_point, hit_normal, light, hitObject->material, scene);
            }
            total_light = total_light + computeLightingDirectional(hit_point, hit_normal, scene.dirLight, hitObject->material, scene);
            return total_light;
        }

        Vec3 unit_direction = r.direction.normalize();
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
    }

    static Vec3 computeLighting(const Vec3& hit_point, const Vec3& normal, const PointLight& light, const Material& material, const Scene& scene) {
        int shadow_samples = 64;
        int visible_samples = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> rand_offset(-light.radius, light.radius);

        for (int i = 0; i < shadow_samples; i++) {
            Vec3 jittered_light_pos = light.position + Vec3(rand_offset(gen), rand_offset(gen), rand_offset(gen));
            Vec3 light_dir = (jittered_light_pos - hit_point).normalize();
            Ray shadow_ray(hit_point + normal * 0.001f, light_dir);

            bool in_shadow = false;
            for (const auto& object : scene.objects) {
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

        float visibility = float(visible_samples) / float(shadow_samples);
        Vec3 light_dir = (light.position - hit_point).normalize();
        float diffuse = std::max(0.0f, normal.dot(light_dir));
        Vec3 reflection = (2 * normal.dot(light_dir) * normal - light_dir).normalize();
        float specular = std::pow(std::max(0.0f, reflection.dot(-hit_point.normalize())), material.shininess);
        
        return visibility * (material.diffuse * diffuse + material.specular * specular) * light.color * light.intensity;
    }

    static Vec3 computeLightingDirectional(const Vec3& hit_point, const Vec3& normal, const DirectionalLight& dirLight, const Material& material, const Scene& scene) {
        int shadow_samples = 64;
        int visible_samples = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> rand_offset(-dirLight.radius, dirLight.radius);

        for (int i = 0; i < shadow_samples; i++) {
            Vec3 jittered_dir = (dirLight.direction + Vec3(rand_offset(gen), rand_offset(gen), rand_offset(gen))).normalize();
            Ray shadow_ray(hit_point + normal * 0.001f, jittered_dir);

            bool in_shadow = false;
            for (const auto& object : scene.objects) {
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

        float visibility = float(visible_samples) / float(shadow_samples);
        Vec3 light_dir = dirLight.direction.normalize();
        float diffuse = std::max(0.0f, normal.dot(light_dir));
        Vec3 reflection = (2 * normal.dot(light_dir) * normal - light_dir).normalize();
        float specular = std::pow(std::max(0.0f, reflection.dot(-hit_point.normalize())), material.shininess);
        
        return visibility * (material.diffuse * diffuse + material.specular * specular) * dirLight.color;
    }
};

#endif
