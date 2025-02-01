#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "ray.h"
#include <random>
#include <cmath>

class RayTracer {
public:
    static Vec3 trace(const Ray& r, const Scene& scene, int shadow_samples, int depth = 5) {
        if (depth <= 0) return Vec3(0, 0, 0);  // Stop recursion

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

        // If no object is hit, return background color
        if (!hitObject) {
            Vec3 unit_direction = r.direction.normalize();
            float t = 0.5f * (unit_direction.y + 1.0f);
            return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
        }

        Vec3 hit_point = r.at(closest_t);
        Vec3 total_light(0, 0, 0);

        for (const auto& light : scene.pointLights) {
            total_light = total_light + computeLighting(hit_point, hit_normal, light, hitObject->material, scene, shadow_samples);
        }
        
        total_light = total_light + computeLightingDirectional(hit_point, hit_normal, scene.dirLight, hitObject->material, scene, shadow_samples);

        total_light = total_light * hitObject->material.diffuse;

        Vec3 reflected_color(0, 0, 0);
        Vec3 refracted_color(0, 0, 0);

        float fresnel_coeff = fresnel(r.direction, hit_normal, hitObject->material.ior);

        if (hitObject->material.ior > 1.0f) {
            Vec3 refracted_dir;
            bool can_refract = refract(r.direction, hit_normal, hitObject->material.ior, refracted_dir);

            if (!can_refract || fresnel_coeff > 0.95f) {  
                Vec3 reflected_dir = r.direction - 2 * r.direction.dot(hit_normal) * hit_normal;
                Ray reflected_ray(hit_point + hit_normal * 0.001f, reflected_dir);
                return trace(reflected_ray, scene, shadow_samples, depth - 1);
            } else if (fresnel_coeff < 0.05f) {  
                Ray refracted_ray(hit_point - hit_normal * 0.001f, refracted_dir);
                return trace(refracted_ray, scene, shadow_samples, depth - 1);
            } else { 
                if ((rand() / (RAND_MAX + 1.0)) < fresnel_coeff) {  // Reflect
                    Vec3 reflected_dir = r.direction - 2 * r.direction.dot(hit_normal) * hit_normal;
                    Ray reflected_ray(hit_point + hit_normal * 0.001f, reflected_dir);
                    return trace(reflected_ray, scene, shadow_samples, depth - 1);
                } else {  // Refract
                    Ray refracted_ray(hit_point - hit_normal * 0.001f, refracted_dir);
                    return trace(refracted_ray, scene, shadow_samples, depth - 1);
                }
            }
        }

        return total_light;
    }

    static bool refract(const Vec3& incident, Vec3 normal, float ior, Vec3& refracted) {
        float cos_theta = std::min(-incident.dot(normal), 1.0f);
        float eta_ratio = (cos_theta > 0) ? (1.0f / ior) : ior;  // Use different ratios when entering/exiting
        if (cos_theta > 0) normal = -normal;  // Flip normal if exiting material
    
        float k = 1 - eta_ratio * eta_ratio * (1 - cos_theta * cos_theta);
        if (k < 0) return false;  // Total internal reflection
    
        refracted = eta_ratio * incident + (eta_ratio * cos_theta - std::sqrt(k)) * normal;
        return true;
    }

    static float fresnel(const Vec3& incident, const Vec3& normal, float ior) {
        float cos_theta = std::max(-incident.dot(normal), 0.0f);
        float eta_i = 1.0f;  // Air IOR
        float eta_t = ior;   // Material IOR

        if (cos_theta > 0) std::swap(eta_i, eta_t);

        float F0 = std::pow((eta_i - eta_t) / (eta_i + eta_t), 2.0f);  // Base Reflectivity
        return F0 + (1.0f - F0) * std::pow(1.0f - cos_theta, 5.0f);
    }

    static Vec3 computeLighting(const Vec3& hit_point, const Vec3& normal, const PointLight& light, const Material& material, const Scene& scene, int shadow_samples) {
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
                    if (object->material.ior == 1.0f) {
                        in_shadow = true;
                        break;
                    }
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
    

    static Vec3 computeLightingDirectional(const Vec3& hit_point, const Vec3& normal, const DirectionalLight& dirLight, const Material& material, const Scene& scene, int shadow_samples) {
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
                    if (object->material.ior == 1.0f) {
                        in_shadow = true;
                        break;
                    }
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
