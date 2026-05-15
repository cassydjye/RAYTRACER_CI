# Reflection

## How It Works

When a ray hits a reflective surface, a secondary **reflection ray** is spawned in the mirror direction.
The color returned by that ray is blended with the surface's own color.

```
              reflected ray →
             /
            / angle of reflection
           /
──────────●────────────  surface
           \
            \ angle of incidence
             \
              incoming ray
```

Angle of incidence = angle of reflection (mirror law).

## Reflection Direction Formula

```
R = I − 2(N·I)N
```

- `I` = normalized incoming ray direction
- `N` = surface normal
- `R` = reflected ray direction

Example: ray pointing straight down `(0,−1,0)`, normal pointing up `(0,1,0)`:
```
R = (0,−1,0) − 2×(−1)×(0,1,0) = (0,1,0)   ← ray bounces straight up
```

## Color Blending

```
finalColor = surfaceColor × (1 − kr) + reflectedColor × kr
```

- `kr` = reflectivity (0–1)
- `surfaceColor` = local Phong shading result
- `reflectedColor` = full recursive `traceRay()` call in reflected direction

| `kr` | Effect |
|------|--------|
| `0.0` | No reflection (default, uses `FlatColor`) |
| `0.3` | Subtle sheen |
| `0.6` | Clearly reflective |
| `1.0` | Perfect mirror, surface color invisible |

## Depth Limiting

Reflection rays can hit other reflective surfaces → recursive bounces.
Two mirrors facing each other would recurse infinitely without a limit.

Maximum depth = **8 bounces**. At depth 8, recursion stops and returns the local color only.

```
primary ray → bounce 1 → bounce 2 → ... → bounce 8 → no more reflection
```

## Material

When `reflectivity > 0` in the config, `makeMaterial()` returns a `ReflectiveMaterial` instead of `FlatColor`.

```cpp
class ReflectiveMaterial : public IMaterial {
    double getReflectivity() const override { return _reflectivity; }
    // also supports specular, shininess, color
};
```

`FlatColor::getReflectivity()` always returns `0.0` → reflection code path is skipped entirely for non-reflective surfaces.

## Configuration

```cfg
// Reflective plane (ground mirror)
planes = (
    { x = 0; y = -1; z = 0; nx = 0; ny = 1; nz = 0;
      color = { r = 200; g = 200; b = 200; };
      reflectivity = 0.6; }
);

// Mirror sphere
spheres = (
    { x = 0; y = 0; z = -2; r = 0.5;
      color = { r = 220; g = 220; b = 220; };
      reflectivity = 0.9; specular = 0.8; shininess = 128; }
);
```

## Performance

Each reflective surface multiplies the number of rays per pixel by (up to) the bounce depth.
Recommendation: use reflection on **planes and spheres**; avoid on large triangle meshes.
With `samples = 4` and `reflectivity = 0.8` on a sphere: each pixel can fire up to `4 × 8 = 32` rays.
