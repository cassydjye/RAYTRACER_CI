# Phong Shading (Smooth Normals)

## Flat vs Smooth Shading

**Flat shading** assigns one normal per triangle (the geometric face normal).
Every point on the triangle has the same normal → uniform lighting → faceted appearance.

**Phong shading** assigns a normal per vertex and **interpolates** across the triangle surface.
Each point gets a slightly different normal → smooth lighting gradient → curved appearance.

```
Flat:                    Phong:
┌─────────┐              ┌─────────┐
│  same N │              │ N varies│
│ all over│              │ smoothly│
└─────────┘              └─────────┘
Looks like a polygon     Looks like a curve
```

## How It Works

### 1. Vertex Normals

When loading an OBJ file, vertex normals come from either:
- **File normals** (`vn` lines in the OBJ) — highest quality, set by the 3D modeller
- **Computed smooth normals** — if the OBJ has no `vn` lines, the loader averages geometric normals from all faces sharing each vertex

```
Vertex V shared by 3 faces:
smoothNormal[V] = normalize(faceNormal1 + faceNormal2 + faceNormal3)
```

### 2. Barycentric Interpolation

At the ray-triangle intersection point, barycentric coordinates `(u, v, w)` describe position within the triangle:

```
w = 1 - u - v   (weight of vertex 0)
u              (weight of vertex 1)
v              (weight of vertex 2)
```

The interpolated normal at the hit point:
```
N = normalize(w·N0 + u·N1 + v·N2)
```

This happens in `Triangles::hits()` when `_hasVertexNormals = true`.

### 3. Normal Flip

If the interpolated normal points away from the incoming ray, it is flipped.
This ensures correct lighting for both front and back faces:

```cpp
if (N.dot(rayDirection) > 0.0)
    N = -N;
```

## Effect on Rendering

Phong shading is most visible on **medium-poly meshes** (a few hundred to a few thousand triangles).

- **High-poly mesh** → face normals already nearly smooth → minimal visual difference
- **Low-poly mesh** → dramatic difference: flat gives a "crystal" look, smooth gives a "curved" look
- **Single triangle** → always flat (only one face, no neighbours to average with)

## OBJ Loader Behaviour

| OBJ content | Normals used |
|-------------|--------------|
| Has `vn` lines, face uses `v//vn` format | File normals |
| Has `vn` lines, face uses `v` format only | Computed smooth normals (fallback) |
| No `vn` lines | Computed smooth normals |

Smooth normals are **always computed** as a fallback, even when file normals exist, to safely handle mixed-format faces.
