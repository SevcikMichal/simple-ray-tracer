#include <iostream>
#include <fstream>

int main() {
    const int width = 800, height = 800;
    std::ofstream image("output.ppm");

    image << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float r = float(i) / width;
            float g = float(j) / height;
            float b = 0.2;

            int ir = int(255 * r);
            int ig = int(255 * g);
            int ib = int(255 * b);

            image << ir << " " << ig << " " << ib << "\n";
        }
    }

    image.close();
    std::cout << "Image rendered: output.ppm\n";
    return 0;
}
