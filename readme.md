# RayTracer

A simple ray tracer written in C++ that generates images using basic ray tracing techniques. This is a learning project to refresh knowledge in computer graphics.

## Getting Started

### Prerequisites
- C++17 or later
- CMake
- A C++ compiler (Clang, GCC, or MSVC)

### Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/RayTracer.git
   cd RayTracer
   ```
2. Build the project:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```

### Running the Ray Tracer
Execute the program to generate an output image:
```sh
./RayTracer
```
This will create an `output.ppm` file in the project directory.

### Viewing the Output
To view the generated image, you can use an image viewer that supports PPM files or convert it to PNG using ImageMagick:
```sh
convert output.ppm output.png
```