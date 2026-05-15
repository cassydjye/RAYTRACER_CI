# Scene Configuration Guide

Scene files use [libconfig](https://hyperrealm.github.io/libconfig/) syntax (`.cfg`).
Run: `./raytracer scenes/my_scene.cfg > output.ppm`

---

## Camera

```cfg
camera:
{
    resolution  = { width = 1920; height = 1080; };
    position    = { x = 0; y = 0; z = 5; };
    rotation    = { x = 0; y = 0; z = 0; };  // degrees, XYZ Euler
    fieldOfView = 60.0;                        // degrees
    samples     = 1;                           // anti-aliasing rays per pixel (1 = off, 4/16 = smooth)
};
```

| Field | Type | Description |
|-------|------|-------------|
| `resolution` | group | Output image dimensions in pixels |
| `position` | group | Camera world-space position |
| `rotation` | group | Camera orientation in degrees (XYZ Euler) |
| `fieldOfView` | float | Horizontal FOV in degrees |
| `samples` | int | Anti-aliasing samples. Render time scales linearly. |

---

## Primitives

All primitives support these optional material fields:

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `color` | group `{r g b}` | required | Surface color, values 0–255 |
| `specular` | float 0–1 | `0.0` | Phong specular intensity |
| `shininess` | float | `32.0` | Specular highlight tightness (higher = smaller hotspot) |
| `reflectivity` | float 0–1 | `0.0` | Mirror reflectivity. Values > 0 activate `ReflectiveMaterial`. |

All primitives also support optional transform fields:

| Field | Type | Description |
|-------|------|-------------|
| `rotation` | group `{x y z}` | Rotation in degrees around the primitive's origin |
| `translation` | group `{x y z}` | World-space offset applied after rotation |

---

### Sphere

```cfg
spheres = (
    {
        x = 0; y = 0; z = -2;
        r = 0.5;
        color = { r = 255; g = 100; b = 50; };
        specular = 0.8; shininess = 32;
        reflectivity = 0.3;
    }
);
```

| Field | Description |
|-------|-------------|
| `x y z` | Center position |
| `r` | Radius |

---

### Plane

Infinite flat surface defined by a point and a normal vector.

```cfg
planes = (
    {
        x = 0; y = -1; z = 0;
        nx = 0; ny = 1; nz = 0;
        color = { r = 200; g = 200; b = 200; };
        reflectivity = 0.4;
    }
);
```

| Field | Description |
|-------|-------------|
| `x y z` | Any point on the plane |
| `nx ny nz` | Surface normal direction (does not need to be unit length) |

---

### Cylinder

```cfg
cylinders = (
    {
        x = 0; y = -1; z = -2;
        ax = 0; ay = 1; az = 0;
        r = 0.3; h = 2.0;
        color = { r = 80; g = 200; b = 120; };
        specular = 0.5; shininess = 16;
        rotation = { x = 0; y = 0; z = 30; };
    }
);
```

| Field | Description |
|-------|-------------|
| `x y z` | Base center position |
| `ax ay az` | Axis direction |
| `r` | Radius |
| `h` | Height |

---

### Triangle

Single triangle defined by three vertices. Flat-shaded (no smooth normals).

```cfg
triangles = (
    {
        v0 = { x =  0.0; y =  0.5; z = -1.0; };
        v1 = { x = -0.5; y = -0.5; z = -1.0; };
        v2 = { x =  0.5; y = -0.5; z = -1.0; };
        color = { r = 255; g = 200; b = 0; };
        specular = 0.6; shininess = 8;
    }
);
```

---

### Mesh (OBJ file)

Loads a `.obj` file. Supports triangles and n-gons (fan-triangulated).
Smooth normals are interpolated automatically (Phong shading).

```cfg
meshes = (
    {
        file     = "model.obj";
        position = { x = 0.0; y = -1.0; z = -2.0; };
        rotation = { x = 0; y = 45; z = 0; };
        translation = { x = 1; y = 0; z = 0; };
        color    = { r = 180; g = 120; b = 80; };
        specular = 0.6; shininess = 16;
        reflectivity = 0.2;
    }
);
```

| Field | Description |
|-------|-------------|
| `file` | Path to `.obj` file (relative to working directory) |
| `position` | Offset added to all vertices at load time |
| `rotation` | Rotation around `position` in degrees |
| `translation` | Additional world-space translation after rotation |

**OBJ support:** `v` (vertices), `vn` (normals), `f` (faces).
UV coordinates (`vt`) and material files (`mtllib`/`usemtl`) are ignored.

### Mesh (inline triangles)

```cfg
meshes = (
    {
        position = { x = 0; y = 0; z = 0; };
        color = { r = 200; g = 80; b = 40; };
        triangles = (
            { v0 = { x=0; y=1; z=0; }; v1 = { x=-1; y=0; z=0; }; v2 = { x=1; y=0; z=0; }; },
            { v0 = { x=0; y=1; z=0; }; v1 = { x=1; y=0; z=0; }; v2 = { x=0; y=0; z=1; }; }
        );
    }
);
```

---

## Lights

```cfg
lights:
{
    ambient = 0.3;
    diffuse = 0.85;

    directional = (
        { x = -1.0; y = 2.0; z = 1.0; }
        { x = 1.0; y = 0.5; z = -1.0; intensity = 0.5; }
    );

    point = (
        { x = 2.0; y = 3.0; z = 0.0; intensity = 5.0; }
    );
};
```

| Field | Description |
|-------|-------------|
| `ambient` | Flat background brightness added to every surface (0–1) |
| `diffuse` | Default intensity for directional lights without explicit `intensity` |
| `directional` | Parallel light (like the sun). `x y z` = direction toward the light. |
| `point` | Positional light with inverse-square falloff. `x y z` = world position. |

**Point light intensity:** falls off as `1/d²`. Use values of `2.0`–`10.0` for visible effect.
**Multiple lights:** any number of directional and point lights can be combined.

---

## Full Example

```cfg
camera:
{
    resolution  = { width = 1920; height = 1080; };
    position    = { x = 0; y = 0.5; z = 4; };
    rotation    = { x = -5; y = 0; z = 0; };
    fieldOfView = 60.0;
    samples     = 4;
};

primitives:
{
    spheres = (
        { x = -1; y = 0; z = 0; r = 0.5;
          color = { r = 255; g = 80; b = 80; };
          specular = 0.9; shininess = 64; reflectivity = 0.3; }
    );

    planes = (
        { x = 0; y = -0.5; z = 0; nx = 0; ny = 1; nz = 0;
          color = { r = 180; g = 180; b = 180; };
          reflectivity = 0.2; }
    );

    meshes = (
        { file = "model.obj";
          position = { x = 1; y = -0.5; z = -1; };
          rotation = { x = 0; y = 30; z = 0; };
          color = { r = 100; g = 180; b = 255; };
          specular = 0.7; shininess = 32; }
    );
};

lights:
{
    ambient = 0.3;
    diffuse = 0.8;
    directional = (
        { x = 1; y = 2; z = 1; }
    );
    point = (
        { x = -2; y = 3; z = 2; intensity = 4.0; }
    );
};
```
