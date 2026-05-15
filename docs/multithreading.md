# Multithreading (OpenMP)

## Implementation

The render loop parallelizes across **rows** using OpenMP:

```cpp
#pragma omp parallel for schedule(dynamic)
for (int y = h - 1; y >= 0; --y) {
    // each thread processes one row at a time
}
```

`schedule(dynamic)` means threads grab the next available row when they finish one.
This is better than static scheduling because some rows (e.g. complex geometry) take longer than others.

## Race Condition Prevention

The renderer writes pixels into a **flat buffer** first, then outputs the PPM sequentially after the parallel section.

```cpp
std::vector<Color> buffer(w * h);   // pre-allocated, indexed by (y*w + x)

#pragma omp parallel for
for (int y ...) {
    buffer[(h-1-y) * w + x] = traceRay(...);   // each thread writes to its own row
}

// sequential PPM output — no race condition
for (const Color& c : buffer) out << ...;
```

Each thread writes to a unique index in `buffer` → no two threads share a memory location.

## Per-Thread RNG

Anti-aliasing requires random numbers per ray. Using a shared RNG would need a lock.
Instead, each row gets its own RNG seeded by the row index:

```cpp
std::mt19937 rng(static_cast<unsigned>(y * 1000003));
std::uniform_real_distribution<double> dist(0.0, 1.0);
```

No contention, fully deterministic output for the same seed.

## Build Flags

```makefile
CXXFLAGS = -fopenmp
LDFLAGS  = -lgomp
```

## Thread Count

OpenMP defaults to the number of logical CPU cores.
Override at runtime:

```sh
OMP_NUM_THREADS=4 ./raytracer scene.cfg > out.ppm
```

## What Is NOT Parallelized

| Phase | Parallel? | Reason |
|-------|-----------|--------|
| Scene parsing | No | Fast, single-threaded |
| OBJ loading | No | Fast, I/O bound |
| BVH construction | No | Built once before render |
| Pixel rendering | **Yes** | Main bottleneck |
| PPM output | No | Sequential by format |

## Speedup

Near-linear scaling on CPU-bound renders. With 8 cores and a complex scene:
expected ~6–7× speedup (some overhead from thread management and cache effects).
