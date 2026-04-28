# How the Raytracer Works

A step-by-step walkthrough of every stage, from startup to the final pixel written to stdout.

---

## Overview

A raytracer works backwards from the camera: instead of simulating light leaving a source and bouncing around the scene, it asks *"what colour does this pixel see?"* by shooting a ray from the camera through each pixel and finding what it hits.

```
Camera
  │
  │  one ray per pixel
  ▼
Virtual screen (Rectangle3D)
  │
  │  ray travels into the scene
  ▼
Sphere intersection test
  │
  ├─ HIT  →  compute surface normal  →  Lambertian shading  →  red pixel
  └─ MISS →  sky gradient                                   →  blue pixel
  │
  ▼
PPM colour written to stdout
```

---

## Step 1 — Setting up the scene

```cpp
// main.cpp
Math::Point3D cameraOrigin(0, 0, 0);

RayTracer::Rectangle3D screen(
    Math::Point3D(-2, -1, -1),   // bottom-left corner of the virtual screen
    Math::Vector3D(4, 0, 0),     // horizontal span (width = 4 units)
    Math::Vector3D(0, 2, 0)      // vertical span   (height = 2 units)
);

RayTracer::Camera cam(cameraOrigin, screen);
RayTracer::Sphere s(Math::Point3D(0, 0, -1), 0.5);  // sphere 1 unit in front
```

The coordinate system is **right-handed**.  The camera sits at the origin and looks down the **−Z** axis.  The virtual screen is a flat rectangle placed at Z = −1, spanning X ∈ [−2, 2] and Y ∈ [−1, 1].

---

## Step 2 — Iterating over pixels

```cpp
for (int y = imageHeight - 1; y >= 0; --y) {     // top row first in screen space
    for (int x = 0; x < imageWidth; ++x) {
        double u = (double)x / (imageWidth  - 1); // [0, 1] left → right
        double v = (double)y / (imageHeight - 1); // [0, 1] bottom → top
```

`u` and `v` are **normalised coordinates** in [0, 1].  They will be used to look up the corresponding world-space point on the virtual screen.

The outer loop counts *down* from `imageHeight - 1` so that pixel row 0 in image space corresponds to the bottom of the screen (positive Y in world space).

---

## Step 3 — Generating a ray (`Camera::ray`)

```cpp
// Camera.cpp
Ray Camera::ray(double u, double v) const {
    return Ray(origin, screen.pointAt(u, v) - origin);
}
```

`screen.pointAt(u, v)` interpolates a point on the virtual screen:

```
P = origin + bottom_side * u + left_side * v
  = (-2, -1, -1) + (4,0,0)*u + (0,2,0)*v
```

The ray **direction** is the vector from the camera to that screen point.  It is *not* normalised yet — normalisation happens later, only when the math requires it (e.g. shading).

---

## Step 4 — Ray–sphere intersection (`Sphere::hits`)

We want to find `t` such that the point `P(t) = origin + t·direction` lies on the sphere surface:

```
|P(t) − center|² = radius²
```

Substituting `P(t)` and letting `oc = origin − center`:

```
|direction|²·t²  +  2·(oc·direction)·t  +  (|oc|² − radius²)  =  0
     a                      b                         c
```

This is a standard quadratic in `t`.  The **discriminant** `b² − 4ac` tells us:

| discriminant | meaning |
|---|---|
| < 0 | ray misses the sphere entirely |
| = 0 | ray is tangent (one intersection) |
| > 0 | ray passes through (two intersections: front face and back face) |

```cpp
// sphere.cpp — hits(ray, t)
double sqrtD = std::sqrt(discriminant);
double t0 = (-b - sqrtD) / (2.0 * a);   // nearer root  (front face)
double t1 = (-b + sqrtD) / (2.0 * a);   // farther root (back face / inside)

if (t0 > 0.001) { t = t0; return true; }  // prefer front face
if (t1 > 0.001) { t = t1; return true; }  // fall back to back face
```

The `0.001` threshold avoids **shadow acne**: floating-point imprecision can place the hit point fractionally *inside* the surface, causing a ray to immediately re-intersect itself.

---

## Step 5 — Computing the surface normal

```cpp
Math::Point3D hitPoint = r.at(t);                        // P(t)
Math::Vector3D normal   = (hitPoint - s.center).normalize(); // outward unit normal
```

`hitPoint − center` points radially outward from the sphere's centre to the surface.  Dividing by `radius` (via `normalize()`) gives a **unit normal** — a vector of length 1 pointing away from the surface, which is required for lighting math.

---

## Step 6 — Lambertian diffuse shading

Lambertian (matte) shading models a surface that scatters light equally in all directions.  The brightness depends only on the angle between the surface normal and the incoming light direction.

```cpp
Math::Vector3D lightDir = Math::Vector3D(1.0, 1.0, 0.5).normalize();
const double ambient = 0.15;

double diffuse   = std::max(0.0, normal.dot(lightDir));
double intensity = ambient + (1.0 - ambient) * diffuse;
```

- `normal.dot(lightDir)` is `cos(θ)` where θ is the angle between them.  It is 1 when the surface faces the light directly, 0 when it is perpendicular, and negative when it faces away (clamped to 0).
- `ambient` is a small constant added so shadow-side faces are never completely black.
- `intensity` is in [0.15, 1.0].

The final colour applies intensity to a base red `(0.8, 0.2, 0.2)`:

```cpp
toColor(0.8 * intensity)   // R
toColor(0.2 * intensity)   // G
toColor(0.2 * intensity)   // B
```

---

## Step 7 — Sky background (ray miss)

When a ray hits nothing, the background colour is a gradient that blends from white (bottom, horizon) to light blue (top):

```cpp
double blend = 0.5 * (r.direction.normalize().y + 1.0);  // [0, 1], 0 = horizon
// colour interpolates between (0.53, 0.81, 0.92) at horizon and (1, 1, 1) at top
```

The `.y` component of the normalised ray direction acts as a vertical angle: rays aimed straight up have y = 1, rays aimed straight down have y = −1, and rays aimed horizontally have y ≈ 0.

---

## Step 8 — Writing PPM output

```cpp
std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
// ... per pixel:
std::cout << R << " " << G << " " << B << "\n";
```

**PPM (Portable Pixmap)** is the simplest possible image format: a plain-text header followed by one RGB triple per pixel, left-to-right, top-to-bottom.  No library needed.

`toColor` clamps a [0, 1] double to the [0, 255] integer range:

```cpp
static int toColor(double v) {
    return static_cast<int>(std::clamp(v, 0.0, 1.0) * 255.99);
}
```

The `255.99` (rather than `256`) keeps the value from rounding up to 256 due to floating-point drift.

Redirect stdout to a file to get a viewable image:

```bash
./raytracer > output.ppm
```

---

## Data flow summary

```
pixel (x, y)
    │
    │  normalise → (u, v)
    ▼
screen.pointAt(u, v)          →  world-space point on virtual screen
    │
    │  subtract camera origin
    ▼
Ray { origin, direction }
    │
    │  solve quadratic at² + bt + c = 0
    ▼
t (intersection distance)
    │
    │  ray.at(t)
    ▼
hitPoint                      →  3D position on sphere surface
    │
    │  (hitPoint - center).normalize()
    ▼
normal                        →  unit vector pointing away from sphere
    │
    │  dot(normal, lightDir)
    ▼
intensity                     →  ambient + diffuse factor ∈ [0.15, 1.0]
    │
    │  multiply base colour, clamp, scale to [0, 255]
    ▼
PPM pixel (R, G, B)
```
