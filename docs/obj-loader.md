# OBJ Loader

## Supported Format

The loader reads ASCII `.obj` files and ignores everything it doesn't understand.

| Token | Parsed | Example |
|-------|--------|---------|
| `v` | Vertex position | `v 1.0 2.5 -0.3` |
| `vn` | Vertex normal | `vn 0.0 1.0 0.0` |
| `f` | Face (triangle or n-gon) | `f 1 2 3` or `f 1/1/1 2/2/2 3/3/3` |
| `#` | Comment | `# this is ignored` |
| Everything else | Ignored | `vt`, `g`, `o`, `mtllib`, `usemtl`, `s` |

### Face Token Formats

All four OBJ face formats are supported:

| Format | Example | UV used? |
|--------|---------|----------|
| Vertex only | `f 1 2 3` | — |
| Vertex + UV | `f 1/1 2/2 3/3` | UV discarded |
| Vertex + Normal | `f 1//1 2//2 3//3` | ✓ |
| Vertex + UV + Normal | `f 1/1/1 2/2/2 3/3/3` | UV discarded |

### N-gon Triangulation

Faces with more than 3 vertices are **fan-triangulated**:

```
quad (4 verts) → 2 triangles
pentagon       → 3 triangles
...
f v0 v1 v2 v3 → (v0,v1,v2) + (v0,v2,v3)
```

## Normal Handling

| OBJ content | Result |
|-------------|--------|
| Has `vn`, face uses `v//vn` | Normals from file → best quality |
| Has `vn`, face uses `v` only | Computed smooth normals (fallback) |
| No `vn` lines | Computed smooth normals |

**Smooth normal computation:**
1. For each face, compute the geometric normal (cross product of two edges)
2. Add it to each of the face's vertex accumulators
3. Normalize each accumulator → smooth normal per vertex

Result: curved surfaces appear smooth even on medium-poly meshes.

## Bounds Checking

All face indices are validated before use. Out-of-range or negative indices are **silently skipped** — the triangle is not added to the mesh.

This prevents segfaults from malformed OBJ files.

## Position and Transform

Vertices are offset by `position` at load time:

```cfg
meshes = ({
    file     = "model.obj";
    position = { x = 1; y = 0; z = -2; };   // added to every vertex
    rotation = { x = 0; y = 45; z = 0; };   // applied via Transform wrapper
    translation = { x = 0.5; y = 0; z = 0; }; // additional offset post-rotation
});
```

`position` bakes the offset into vertex coordinates at load time.
`rotation` and `translation` wrap the Mesh in a `Transform` that modifies rays at render time.

## Internal BVH

Each `Mesh` automatically builds an internal BVH over its triangles at construction.
A 100k-triangle mesh reduces from 100k intersection tests per ray to ~17.

See [bvh.md](bvh.md) for details.

## Exporting from Blender

1. File → Export → Wavefront (.obj)
2. Check **Triangulate Faces** (avoids n-gon issues)
3. Check **Export Normals** (enables Phong shading)
4. Uncheck **Write Materials** (ignored anyway)
5. Place the `.obj` file in the raytracer's working directory
