#include "raytracer.h"
#include "sphere.h"
#include <fstream>
#include <memory>

int main() {
    const int width = 800, height = 400;
    Vec3 lower_left(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);

    Scene scene(DirectionalLight(Vec3(-1, 1, 1), Vec3(0.8, 0.8, 0.8), 0.2f));

    Material red(Vec3(1, 0.1, 0.1), Vec3(1, 1, 1), 32.0f);
    Material green(Vec3(0.1, 1, 0.1), Vec3(1, 1, 1), 16.0f);

    scene.addObject(std::make_unique<Sphere>(Vec3(0, 0, -1), 0.5, red));
    scene.addObject(std::make_unique<Sphere>(Vec3(1, -0.5, -1.5), 0.3, green));

    scene.addPointLight(PointLight(Vec3(1, 1, 0), Vec3(1, 1, 0.1), 1.5f, 0.1f));
    scene.addPointLight(PointLight(Vec3(-1, 2, -1), Vec3(0.1, 1, 0.1), 1.2f, 0.4f));

    std::ofstream image("output.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    int shadow_samples = 16;  // Change this to adjust softness of shadows

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            Ray r(origin, lower_left + (float(i) / width) * horizontal + (float(j) / height) * vertical);
            Vec3 col = RayTracer::trace(r, scene, shadow_samples);
            image << int(255.99 * col.x) << " " << int(255.99 * col.y) << " " << int(255.99 * col.z) << "\n";
        }
    }

    image.close();
    return 0;
}
