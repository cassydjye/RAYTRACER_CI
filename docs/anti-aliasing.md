# Anti-Aliasing

## The Problem

Without anti-aliasing, each pixel fires exactly one ray through its center.
At geometry edges, a pixel is either fully inside or fully outside a surface — producing jagged "staircase" edges (aliasing).

```
Without AA:         With AA (x4):
█░░░░░              ▓▒░░░░
██░░░░              █▓▒░░░
███░░░              ██▓▒░░
```

## Implementation — MSAA (Multi-Sample Anti-Aliasing)

Instead of one ray per pixel, fire `N` rays at random sub-pixel positions and average the results.

```
Pixel boundary
┌───────────┐
│  ·  ·     │  ← 4 sample points, randomly jittered
│     ·  ·  │    within the pixel area
└───────────┘
```

Each sample hits a slightly different point on the geometry edge.
The average blends surface color with background → smooth transition.

### Code path

```
render() → per pixel:
    for s in 0..samples:
        u = (x + random()) / (width  - 1)
        v = (y + random()) / (height - 1)
        accum += traceRay(cam.ray(u, v))
    pixel = accum / samples
```

Each OpenMP thread has its own RNG seeded by row index — no lock contention.

## Configuration

```cfg
camera:
{
    samples = 4;   // rays per pixel
};
```

| Value | Effect | Render time |
|-------|--------|-------------|
| `1` | No AA (default) | 1× |
| `4` | Good balance | 4× |
| `16` | Very smooth | 16× |
| `64` | Near-perfect | 64× |

`samples = 1` disables jitter entirely — single ray through pixel center.

## Trade-off

Render time scales **linearly** with sample count.
With OpenMP already running N threads, effective cost per thread = `samples × pixels / N`.
Combine with BVH for best overall performance.
