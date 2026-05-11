# 🚀 Raytracer

A high-performance C++17 ray tracing engine that renders photorealistic images from scene configuration files. This project implements a complete ray tracing pipeline with support for multiple light sources, materials, and geometric primitives.

---

## ✨ Features

- **Ray Tracing Engine**: Real-time ray casting with accurate light simulation
- **Multiple Primitives**: Support for spheres, planes, cylinders, meshes, and custom triangles
- **Lighting System**: Ambient, directional, and point lights with Lambertian shading
- **Scene Configuration**: Flexible libconfig++ format for describing scenes
- **OBJ Model Support**: Load and render 3D mesh models
- **BVH Acceleration**: Bounding Volume Hierarchy for optimized ray-primitive intersection
---

## 📋 Requirements

- **C++17** or later compiler
- `libconfig++` development library
- GNU Make
- Linux/Unix environment

---

## 🔨 Building

### Quick Start

```bash
make              # Build the raytracer and all plugins
make re           # Clean rebuild
make clean        # Remove object files
make fclean       # Remove all generated files
```

The build process generates:
- `./raytracer` — main executable

---

## ▶️ Usage

### Basic Rendering

```bash
./raytracer <SCENE_FILE> > output.ppm
```

### Example Scenes

```bash
# Render sphere demo
./raytracer scenes/demo_sphere.cfg > sphere.ppm

# Render stick figure
./raytracer scenes/demo_bonhomme.cfg > bonhomme.ppm
```

### Scene Configuration Format

Create a scene file (`.cfg`) with libconfig++ syntax:

```cfg
camera:
{
    resolution = { width = 1920; height = 1080; };
    position   = { x = 0; y = -0.15; z = 2; };
    rotation   = { x = 0; y = 0; z = 0; };
    fieldOfView = 60.0;
};

primitives:
{
    spheres = (
        { x = 0.10;    y = -0.35;  z = -1.6; r = 0.15;  color = { r = 255; g = 165; b = 0; }; specular = 0.8; shininess = 4;}, //PAF DORé (boule 1)
        { x = -0.10; y = -0.35; z = -1.6; r = 0.15; color = { r = 255; g = 165; b = 0; }; specular = 0.8; shininess = 4;}, //PAF DORé (boule 2)
        { x = -0.60;    y = -0.35;  z = -1.6; r = 0.15;  color = { r = 255; g = 165; b = 0; }; specular = 0.8; shininess = 4;}, //PAF DORé (boule 3)
        { x = -0.40; y = -0.35; z = -1.6; r = 0.15; color = { r = 255; g = 165; b = 0; }; specular = 0.8; shininess = 4;}, //PAF DORé (boule 4)
        { x = 0.40;    y = -0.35;  z = -1.6; r = 0.15;  color = { r = 255; g = 165; b = 0; }; specular = 0.8; shininess = 4;}, //PAF DORé (boule 5)
        { x = 0.60; y = -0.35; z = -1.6; r = 0.15; color = { r = 255; g = 165; b = 0; }; specular = 0.8; shininess = 4;} //PAF DORé (boule 6)
    );
    planes = (
        { x = 0; y = -0.5; z = 0; nx = 0; ny = 1; nz = 0; color = { r = 64; g = 64; b = 255; }; }
    );
};

lights:
{
    ambient = 0.45;
    diffuse = 0.85;
    directional = (
        { x = 0.5; y = 1.0; z = -2.0; }
    );
};
```

---

## 🏗️ Project Structure

```
src/
├── core/          Renderer, scene management
├── math/          Vector3D, Point3D, mathematical utilities
├── raytracer/     Ray, Camera, Color, intersection utilities
├── primitives/    All primitives
└── scene/         Scene parsing and building


scenes/           Example scene configuration files
tests/            Unit test suite
include/          Public header files
docs/             Architecture and design documentation
```

---

## 🧪 Testing

```bash
make tests_run        # Run the test suite
```

Unit tests are located in the `tests/` directory and cover core mathematical operations and intersection tests.

---

## 📐 Architecture

This project follows several design patterns:

- **Factory Pattern**: `PrimitiveFactory` and `LightFactory` create objects dynamically
- **Builder Pattern**: `SceneBuilder` assembles scenes incrementally
- **Composite Pattern**: `Scene` manages collections of primitives uniformly

For detailed architecture information, see [ARCHI.md](ARCHI.md).

For how the ray tracing algorithm works, see [PROJECT.md](PROJECT.md).

---

## 💾 Output Format

Convert PPM to other formats using standard tools:

```bash
# Convert PPM to PNG
convert output.ppm output.png

# Convert PPM to JPEG
convert output.ppm output.jpg
```

---

## 🎯 Ray Tracing Fundamentals

The engine works backward from the camera:

1. Cast one ray per pixel through the virtual screen
2. Find the closest intersection with scene primitives
3. Compute surface normals and apply lighting
4. Return the resulting color for that pixel
5. Write all pixels to PPM output

**Coordinate System**: Right-handed, camera looks down the -Z axis by default.

---

## 📦 Dependencies

| Library | Purpose | License |
|---------|---------|---------|
| libconfig++ | Scene file parsing | LGPL |
| C++ STL | Standard library | Part of compiler |

---

## 👨‍💻 Authors

- Raphael Dumon (raphael.dumon@epitech.eu)
- Clement Fabre (clement.fabre@epitech.eu)
- Matheo Emma (matheo.emma@epitech.eu)
- Seedjye Malbrouck (seedjye.malbrouck@epitech.eu)

---

## 📚 Additional Resources

- [Architecture Overview](ARCHI.md)
- [Ray Tracing Algorithm](PROJECT.md)
- [UML Diagrams](uml/)
