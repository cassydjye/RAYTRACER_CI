# Adding a New Primitive

This guide provides step-by-step instructions for implementing a new geometric primitive in the RayTracer. Each primitive must inherit from `IPrimitive` and implement ray-intersection detection.

> **Example primitive name used throughout:** `NewPrimitive`

## Table of Contents
1. [Create Header File](#step-1--create-the-header-file)
2. [Create Source File](#step-2--create-the-source-file)
3. [Configure Parser](#step-3--configure-the-parser)
4. [Integrate with Scene](#step-4--integrate-with-the-scene)
5. [Update Build System](#step-5--update-the-build-system)

---

## Step 1 — Create the Header File

### Location
All primitive headers must be placed in:
```
include/primitives/NewPrimitive.hpp
```

### Requirements
Your primitive class must:
- Inherit from `IPrimitive` interface
- Implement the `hits()` method for ray-intersection testing
- Store position, direction, and material references
- Define default/copy constructors and assignment operators

### Template

```cpp
/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** NewPrimitive
*/

#pragma once

#include <memory>
#include "../math/Vector3D.hpp"
#include "../interfaces/IPrimitive.hpp"
#include "../interfaces/IMaterial.hpp"

#ifndef NEWPRIMITIVE_HPP_
#define NEWPRIMITIVE_HPP_

namespace RayTracer {

    class NewPrimitive : public IPrimitive {
        public:
            NewPrimitive(
                const Math::Point3D& position,
                const Math::Vector3D& direction,
                std::shared_ptr<IMaterial> material
            ) :
                _position(position),
                _direction(direction),
                _material(std::move(material))
            {}

            NewPrimitive() = default;
            ~NewPrimitive() = default;
            NewPrimitive(const NewPrimitive&) = default;
            NewPrimitive& operator=(const NewPrimitive&) = default;

            bool hits(
                const Ray& ray,
                double tMin,
                double tMax,
                HitRecord& rec
            ) const override;

        private:
            Math::Point3D _position;
            Math::Vector3D _direction;
            std::shared_ptr<IMaterial> _material;
    };
}

#endif /* !NEWPRIMITIVE_HPP_ */
```

---

## Step 2 — Create the Source File

### Location
Create the implementation file at:
```
src/primitives/NewPrimitive.cpp
```

### Key Components
- **Intersection Algorithm:** Implement the mathematical ray-intersection test
- **Hit Record:** Populate the `HitRecord` with intersection data
- **Bounds Checking:** Verify `t` falls within `[tMin, tMax]` range

### Template

```cpp
/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** NewPrimitive
*/

#include "../../include/primitives/NewPrimitive.hpp"
#include "../../include/materials/FlatColor.hpp"
#include <cmath>

bool RayTracer::NewPrimitive::hits(
    const Ray& ray,
    double tMin,
    double tMax,
    HitRecord& rec
) const
{
    // Implement ray-primitive intersection algorithm
    // Calculate intersection parameter t

    double t = 0.0;

    // Validate t is within acceptable range
    if (t < tMin || t > tMax)
        return false;

    // Populate hit record with intersection data
    rec.t = t;
    rec.point = ray.at(t);
    rec.normal = _direction;
    rec.material = _material;

    return true;
}
```

---

## Step 3 — Configure the Parser

### Configuration Format
Add your primitive definition to a scene configuration file using this format:

```libconfig
newprimitives = (
{
    x = 0;
    y = 0;
    z = -3;

    dx = 0;
    dy = 1;
    dz = 0;

    color = {
        r = 255;
        g = 0;
        b = 0;
    };
}
);
```

### Field Descriptions
| Field | Type | Purpose |
|-------|------|---------|
| `x, y, z` | double | Position coordinates |
| `dx, dy, dz` | double | Direction vector |
| `color` | object | RGB color (0-255) |

---

## Step 4 — Integrate with the Scene

### Scene Builder Integration
Add instantiation logic to create your primitive:

```cpp
auto material = std::make_shared<FlatColor>(color);

scene.addPrimitive(
    std::make_shared<NewPrimitive>(
        position,
        direction,
        material
    )
);
```

### What This Does
1. Creates a material instance from parsed color data
2. Instantiates the primitive with position, direction, and material
3. Registers the primitive with the scene for rendering

---

## Step 5 — Update the Build System

### Makefile Addition
Add your source file to the build configuration:

```makefile
src/primitives/NewPrimitive.cpp
```

The build system will automatically:
- Compile your implementation
- Link with the raytracer library
- Include necessary headers

---

## Project Structure Overview

After implementation, your primitive follows this structure:

```
include/primitives/
└── NewPrimitive.hpp          ← Interface definition

src/primitives/
└── NewPrimitive.cpp          ← Implementation

scenes/
└── your_scene.cfg            ← Configuration file
```

---

## Implementation Checklist

- [ ] Header file created in `include/primitives/`
- [ ] Class inherits from `IPrimitive`
- [ ] `hits()` method implemented with intersection algorithm
- [ ] Source file created in `src/primitives/`
- [ ] Hit record properly populated (t, point, normal, material)
- [ ] Parser configuration added to scene file
- [ ] Scene builder integration implemented
- [ ] Build system updated with new source file
- [ ] Code compiled without errors
- [ ] Scene renders with new primitive visible

---

# Checklist

- create header file
- create source file
- inherit from IPrimitive
- implement hits()
- add parser support
- add build support

---

# Summary

To add a new primitive:

1. create header
2. create source
3. implement intersection
4. connect parser
5. build project