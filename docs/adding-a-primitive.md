# Adding a New Primitive

This guide walks through adding a new geometric shape to the raytracer.
Example: a `Box` (axis-aligned cube).

## Overview — 4 steps

1. Write the class in `include/primitives/`
2. Implement `hits()` in `src/primitives/`
3. Register a factory in `ConfigParser.cpp`
4. Add the `.cpp` to the `Makefile`

---

## Step 1 — Header (`include/primitives/Box.hpp`)

```cpp
#pragma once

#include <memory>
#include "../interfaces/IPrimitive.hpp"
#include "../interfaces/IMaterial.hpp"

namespace RayTracer {

    class Box final : public IPrimitive {
    public:
        Box(const Math::Point3D& minCorner,
            const Math::Point3D& maxCorner,
            std::shared_ptr<IMaterial> material)
            : _min(minCorner), _max(maxCorner), _material(std::move(material)) {}

        ~Box() override = default;

        bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

        std::optional<AABB> boundingBox() const override {
            return AABB(_min, _max);
        }

    private:
        Math::Point3D _min;
        Math::Point3D _max;
        std::shared_ptr<IMaterial> _material;
    };

}
```

### Rules for the header

- Inherit `IPrimitive` (in `include/interfaces/IPrimitive.hpp`)
- Mark the class `final` unless you plan to subclass it
- Declare `hits()` as `override`
- Implement `boundingBox()` inline if it's trivial — returns an `AABB(min, max)`
- If the primitive is **infinite** (like a plane), omit `boundingBox()` — the base class returns `std::nullopt` by default, which puts it in the unbounded list and skips BVH

---

## Step 2 — Implementation (`src/primitives/Box.cpp`)

```cpp
#include "../../include/primitives/Box.hpp"
#include <algorithm>
#include <cmath>

bool RayTracer::Box::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    // Slab method: intersect ray with 3 pairs of planes
    double tEnter = tMin;
    double tExit  = tMax;
    Math::Vector3D normal;

    const double* bmin = &_min.x;
    const double* bmax = &_max.x;
    const double* orig = &ray.origin.x;
    const double* dir  = &ray.direction.x;

    for (int i = 0; i < 3; ++i) {
        double inv = 1.0 / dir[i];
        double t0  = (bmin[i] - orig[i]) * inv;
        double t1  = (bmax[i] - orig[i]) * inv;
        if (inv < 0.0) std::swap(t0, t1);
        if (t0 > tEnter) { tEnter = t0; /* record which face was entered */ }
        if (t1 < tExit)    tExit  = t1;
        if (tEnter > tExit) return false;
    }

    if (tEnter < tMin) return false;

    rec.t      = tEnter;
    rec.point  = ray.at(tEnter);
    rec.normal = /* compute outward normal for the entered face */ Math::Vector3D(0, 1, 0);
    rec.material = _material;
    return true;
}
```

### What `hits()` must do

| Field | Required | Notes |
|-------|----------|-------|
| `rec.t` | Yes | Distance along the ray — must be in `[tMin, tMax]` |
| `rec.point` | Yes | Use `ray.at(rec.t)` |
| `rec.normal` | Yes | **Outward** unit normal at the hit point |
| `rec.material` | Yes | `shared_ptr<IMaterial>` stored in the primitive |

Return `false` if no hit in `[tMin, tMax]`. Never modify `rec` when returning `false`.

The renderer flips the normal toward the incoming ray automatically — always store the outward normal.

---

## Step 3 — Factory registration (`src/scene/ConfigParser.cpp`)

Inside `makePrimitiveFactory()`, add one `registerType` call:

```cpp
f.registerType("box", [](const libconfig::Setting& s) {
    Math::Point3D minC(asDouble(s["minX"]), asDouble(s["minY"]), asDouble(s["minZ"]));
    Math::Point3D maxC(asDouble(s["maxX"]), asDouble(s["maxY"]), asDouble(s["maxZ"]));
    auto mat = makeMaterial(s);
    auto prim = std::make_unique<RayTracer::Box>(minC, maxC, mat);
    return wrapTransform(std::move(prim), s, minC);
});
```

Add the include at the top of `ConfigParser.cpp`:

```cpp
#include "../../include/primitives/Box.hpp"
```

`wrapTransform` checks for `rotation` and `translation` keys in `s` and wraps the primitive in a `Transform` if needed. Pass a representative origin point as the third argument (used as the pivot).

`makeMaterial(s)` reads `color`, `specular`, `shininess`, `reflectivity` from the setting automatically — nothing extra needed.

### Config usage

The type key is the singular form of the group name in the `.cfg` file:

```cfg
primitives:
{
    // group name "boxes" → strips trailing 's' → type key "box"
    boxes = (
        {
            minX = -1.0; minY = 0.0; minZ = -1.0;
            maxX =  1.0; maxY = 2.0; maxZ =  1.0;
            color = { r = 200; g = 100; b = 50; };
        }
    );
};
```

If your group name doesn't naturally de-pluralize by stripping `s` (e.g. `torus` → `toru`), register the exact key you want and use that exact string as the group name.

---

## Step 4 — Makefile

Add the `.cpp` to the `SRC` list:

```makefile
SRC = \
    ...
    src/primitives/Box.cpp \
    ...
```

---

## BVH integration

`boundingBox()` returning an `AABB` is enough — the top-level BVH is built automatically in `Renderer::render()` before the parallel loop.

- **Finite primitive** → implement `boundingBox()` → goes into BVH
- **Infinite primitive** → omit `boundingBox()` (returns `nullopt`) → goes into the unbounded flat list, tested linearly per ray

Both paths are handled automatically; no renderer changes needed.

---

## Checklist

- [ ] `include/primitives/MyShape.hpp` — class inherits `IPrimitive`, declares `hits()` and `boundingBox()`
- [ ] `src/primitives/MyShape.cpp` — `hits()` fills `rec.t`, `rec.point`, `rec.normal`, `rec.material`
- [ ] `src/scene/ConfigParser.cpp` — `registerType("myshape", ...)` + include added
- [ ] `Makefile` — `src/primitives/MyShape.cpp` added to `SRC`
- [ ] `.cfg` file — group named `myshapes` (or matching key)
