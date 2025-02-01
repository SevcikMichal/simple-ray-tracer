#include "raytracer.h"
#include "sphere.h"
#include <fstream>

int main() {
    const int width = 800, height = 400;
    Vec3 lower_left(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);

    // Create scene
    Scene scene(DirectionalLight(Vec3(-1, 1, 1), Vec3(0.8, 0.8, 0.8)));
    
    // Add objects (currently only spheres)
    scene.addObject(new Sphere(Vec3(0, 0, -1), 0.5, Vec3(1, 1, 1)));  // Red sphere
    scene.addObject(new Sphere(Vec3(1, -0.5, -1.5), 0.3, Vec3(1, 1, 1)));  // Green sphere
    scene.addPointLight(PointLight(Vec3(0.5, 0.5, 0), Vec3(0, 0, .8), 1.5f));

    std::ofstream image("output.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            Ray r(origin, lower_left + (float(i) / width) * horizontal + (float(j) / height) * vertical);
            Vec3 col = RayTracer::trace(r, scene);
            image << int(255 * col.x) << " " << int(255 * col.y) << " " << int(255 * col.z) << "\n";
        }
    }

    image.close();
    
    // Clean up dynamically allocated objects
    for (auto* obj : scene.objects) delete obj;

    return 0;
}
