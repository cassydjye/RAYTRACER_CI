# BVH — Bounding Volume Hierarchy

## The Problem

Without acceleration, every ray tests against every primitive in the scene.
For a scene with N primitives: **O(N) intersection tests per ray**.

A 100k-triangle mesh at 1080p = 100,000 × 2,073,600 ≈ **200 billion tests**.

## Solution — BVH

Wrap groups of primitives in axis-aligned bounding boxes (AABBs).
If a ray misses the box, the entire subtree is skipped.

```
         [root box — whole scene]
              /          \
     [left box]        [right box]
      /      \           /      \
  [prim A] [prim B] [prim C] [prim D]
```

A ray that misses the left box skips both A and B entirely.
Cost per ray: **O(log N)** instead of O(N).

### Real numbers

| Mesh size | Without BVH | With BVH |
|-----------|-------------|----------|
| 1,000 triangles | 1,000 tests | ~10 tests |
| 100,000 triangles | 100,000 tests | ~17 tests |
| 1,000,000 triangles | 1,000,000 tests | ~20 tests |

## AABB — Axis-Aligned Bounding Box

A box defined by `min` and `max` corners, aligned to world axes.
Ray-box intersection uses the **slab method** — tests each axis pair in sequence.

```
     max ●─────────┐
         │         │
         │  object │
         │         │
         └─────────● min
```

If the ray interval `[tMin, tMax]` shrinks to zero on any axis → miss.

## Build Algorithm

1. Compute the centroid of each primitive's AABB
2. Find the axis with the greatest centroid spread (X, Y, or Z)
3. Sort primitives along that axis
4. Split at the median → left and right subtrees
5. Recurse until each leaf contains 1 primitive

Splitting on the widest axis minimizes overlap between children → faster traversal.

## Two-Level BVH

The renderer uses a **two-level** hierarchy:

```
Top-level BVH (over scene primitives)
└── leaf: Transform(Mesh)
         └── Internal BVH (over mesh triangles)  ← built at load time
```

- **Top-level** built in `render()` before the parallel loop
- **Mesh-level** built in `Mesh::Mesh()` at construction time

A ray that hits the Transform's AABB then descends into the Mesh's internal BVH.
For a 100k-triangle car model: ~34 total box tests instead of 100,000 triangle tests.

## Unbounded Primitives

Planes are infinite and have no AABB. They are stored in a separate **flat list** alongside the BVH.
Every ray tests unbounded primitives linearly (typically 0–2 planes per scene — negligible cost).

## Shadow Rays

Shadow rays use `BVH::hitAny()` — exits immediately on the **first** hit found.
No need to find the closest hit → even faster than primary ray traversal.

## Thread Safety

The BVH tree is **read-only** after construction.
Multiple OpenMP threads traverse it concurrently with no locks needed.
