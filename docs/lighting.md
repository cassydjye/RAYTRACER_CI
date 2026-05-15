# Lighting Model

## Overview

The renderer uses the **Phong reflection model**:

```
total = ambient + diffuse + specular
```

Each component is computed per light source and summed.

---

## Ambient Light

Flat, directionless brightness added to every surface regardless of orientation.
Prevents completely black shadow regions.

```cfg
lights: { ambient = 0.3; }
```

- Value in [0, 1]
- Single global value — not per-light

---

## Diffuse (Lambertian)

Brightness based on the angle between the surface normal and the light direction.
Surfaces facing the light are bright; surfaces facing away are dark.

```
diffuse = max(0, N · L) × intensity
```

- `N` = surface normal (unit vector)
- `L` = direction toward light (unit vector)
- Perpendicular surface → `N·L = 0` → no diffuse contribution

```cfg
lights: { diffuse = 0.85; }   // default intensity for directional lights
```

---

## Specular (Phong)

Simulates shiny highlights. Depends on the angle between the reflected light and the viewer.

```
R    = 2(N·L)N − L           // reflect L around N
spec = max(0, R·V)^shininess  // V = direction toward camera
```

- `shininess` controls highlight size: higher = tighter, more mirror-like
- `specular` (ks) scales the intensity

```cfg
{ specular = 0.8; shininess = 64; }  // tight metallic highlight
{ specular = 0.3; shininess = 4;  }  // broad matte sheen
```

Specular highlights are always **white** (simulates white light source).

---

## Shadow Rays

Before computing diffuse/specular, a shadow ray is cast from the hit point toward each light.
If any primitive blocks the ray, the surface is in shadow (diffuse + specular skipped for that light).

```
hit point → light direction → any intersection? → in shadow
```

Shadow rays use `BVH::hitAny()` — exits immediately on first hit (no closest-hit needed).

---

## Directional Light

Parallel rays, infinite distance (like the sun). Direction is constant across the scene.

```cfg
directional = (
    { x = 1; y = 2; z = 1; }                   // uses scene diffuse intensity
    { x = -1; y = 1; z = 0; intensity = 0.4; } // explicit intensity
);
```

`x y z` is the vector **pointing toward** the light source.

---

## Point Light

Positional light with **inverse-square falloff** — intensity = `base / d²`.

```cfg
point = (
    { x = 2.0; y = 3.0; z = 0.0; intensity = 5.0; }
);
```

- Close surfaces are brightly lit; distant surfaces are dark
- Use `intensity` values of `2.0`–`10.0` for visible effect
- Multiple point lights can coexist with directional lights

### Falloff comparison

| Distance | Intensity (`base = 4`) |
|----------|------------------------|
| 1 unit   | 4.0                    |
| 2 units  | 1.0                    |
| 4 units  | 0.25                   |

---

## Multiple Lights

Any combination of ambient, directional, and point lights is supported.
All contributions are summed and clamped to [0, 1] before applying to surface color.

```cfg
lights:
{
    ambient = 0.2;
    diffuse = 0.7;
    directional = (
        { x = 1; y = 2; z = 1; }
    );
    point = (
        { x = -1; y = 3; z = 2; intensity = 3.0; }
        { x =  3; y = 1; z = 0; intensity = 2.0; }
    );
};
```
