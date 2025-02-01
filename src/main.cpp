#include "raytracer.h"
#include "sphere.h"
#include <fstream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

const int width = 800;
const int height = 400;
const int samples_per_pixel = 16;  // Anti-aliasing samples
const int shadow_samples = 8;    // Soft shadow samples
const int max_bounces = 5;      // Reflection max bounces

int num_threads = std::thread::hardware_concurrency();

float aspect_ratio = float(width) / float(height);
float viewport_height = 2.0;
float viewport_width = viewport_height * aspect_ratio;

Vec3 lower_left(-viewport_width / 2, -viewport_height / 2, -1.0);
Vec3 horizontal(viewport_width, 0.0, 0.0);
Vec3 vertical(0.0, viewport_height, 0.0);

Vec3 origin(0.0, 0.0, 0.0);

// Shared buffer for computed colors (avoid file write conflicts)
std::vector<std::vector<Vec3>> image_buffer(height, std::vector<Vec3>(width));

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_offset(0.0f, 1.0f);

void renderRows(int start_row, int end_row, const Scene& scene) {
    for (int j = start_row; j < end_row; j++) {
        for (int i = 0; i < width; i++) {
            Vec3 color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; s++) {
                float u = (i + rand_offset(gen)) / float(width);
                float v = (j + rand_offset(gen)) / float(height);
                Ray r(origin, lower_left + u * horizontal + v * vertical);
                color = color + RayTracer::trace(r, scene, shadow_samples, max_bounces);
            }

            image_buffer[j][i] = color / float(samples_per_pixel);  // Store averaged color
        }
    }
}

int main() {
    if (num_threads == 0) num_threads = 4;  

    Scene scene(DirectionalLight(Vec3(-1, 1, 1), Vec3(0.8, 0.8, 0.8), 0.2f));

    Material red(Vec3(1, 0.1, 0.1), Vec3(1, 1, 1), 32.0f);
    Material green(Vec3(0.1, 1, 0.1), Vec3(1, 1, 1), 16.0f);
    Material mirror(Vec3(1, 1, 1), Vec3(1, 1, 1), 64.0f, 1.0f);
    Material glass(Vec3(1, 1, 1), Vec3(1, 1, 1), 64.0f, 0.1f, 1.52f);

    scene.addObject(std::make_unique<Sphere>(Vec3(0, 0, -2), 0.5, red));
    scene.addObject(std::make_unique<Sphere>(Vec3(1, 0, -2), 0.3, green));
    scene.addObject(std::make_unique<Sphere>(Vec3(-1, 0, -2), 0.3, mirror));
    scene.addObject(std::make_unique<Sphere>(Vec3(0, 0, -1), 0.5, glass));
    
    scene.addPointLight(PointLight(Vec3(1, 1, 0), Vec3(1, 1, 0.1), 1.5f, 0.1f));
    scene.addPointLight(PointLight(Vec3(-1, 2, -1), Vec3(0.1, 1, 0.1), 1.2f, 0.4f));

    // Create and launch threads
    std::vector<std::thread> threads;
    int rows_per_thread = height / num_threads;

    for (int t = 0; t < num_threads; t++) {
        int start_row = t * rows_per_thread;
        int end_row = (t == num_threads - 1) ? height : (t + 1) * rows_per_thread;
        threads.emplace_back(renderRows, start_row, end_row, std::ref(scene));
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Write output file (sequential, avoiding race conditions)
    std::ofstream image("output.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            Vec3& col = image_buffer[j][i];
            image << int(255 * col.x) << " " << int(255 * col.y) << " " << int(255 * col.z) << "\n";
        }
    }

    image.close();
    return 0;
}
