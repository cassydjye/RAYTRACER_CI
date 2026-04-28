# CLAUDE.md — Epitech Raytracer Project

This file instructs Claude Code on how to assist with this codebase. Read it fully before making any non-trivial change.

## Project summary

A C++ raytracer built for the Epitech `G-OOP-400` module. The program takes a scene configuration file, renders the scene by casting rays, and outputs a PPM image to stdout. Concrete primitives and lights are loaded at runtime as `.so` plugins from `./plugins/`.

```bash
./raytracer <SCENE_FILE> > output.ppm
```

Errors go to stderr. Exit code is `84` on error, `0` on success. No other exit codes.

## Non-negotiable rules

1. **Language**: C++17 or later. No C-style casts. No `using namespace std;` in headers.
2. **Allowed libraries only**: C standard library (for `libdl`), C++ standard library, `libconfig++`, SFML (only if we add a GUI). Nothing else. Do not suggest `glm`, `Eigen`, `boost`, `nlohmann::json`, or any other third-party lib.
3. **The core binary links to NONE of the primitive or light implementations.** They are loaded from `./plugins/*.so` at runtime via `libdl`. If you catch yourself writing `#include "Sphere.hpp"` in `src/core/`, stop.
4. **Interfaces are in `src/interfaces/`** and concrete primitives/lights live in `plugins/<name>/`. Each plugin exports a C entry point via `extern "C"`.
5. **Coplien form** for every class that owns resources: default ctor, copy ctor, copy assignment, destructor. Use `= default` or `= delete` explicitly.
6. **RAII for everything.** No raw `new`/`delete` in business logic. Use `std::unique_ptr` for owning, `std::shared_ptr` only when ownership is genuinely shared (primitives in a scene — yes; everything else — probably no).
7. **Error handling**: exceptions for unrecoverable errors, caught at `main`, printed to stderr, return `84`. Do not use exceptions for control flow.
8. **No global state.** No singletons. If you think you need one, you don't.

## Design patterns in use

- **Factory** — `PrimitiveFactory` and `LightFactory` create objects from plugin names read in config.
- **Builder** — `SceneBuilder` assembles a `Scene` incrementally from parsed config.
- **Composite** — `Scene` holds a collection of `IPrimitive` and queries them uniformly.

If you add a pattern, document why in the relevant header.

## Directory map

```
src/core/         — Renderer, Scene, PluginLoader, factories. DO NOT include concrete primitives here.
src/math/         — Vector3D, Point3D, Matrix. No dependency on raytracer/ or core/.
src/raytracer/    — Ray, Camera, Rectangle3D, Color, HitRecord. Depends only on math/.
src/interfaces/   — IPrimitive, ILight, IMaterial, IPlugin. Pure virtual. No implementation.
src/materials/    — FlatColor etc. Implements IMaterial.
src/scene/        — SceneBuilder, ConfigParser. Uses libconfig++.
src/output/       — PPMWriter.
plugins/<name>/   — One directory per plugin. Builds to plugins/<name>.so at the repo root's plugins/.
scenes/           — Example .cfg scene files. Used for manual testing and defense demos.
screenshots/      — Pre-rendered PPMs for defense. Do not commit these if over 5MB.
tests/            — Unit tests. Run them before suggesting a PR is ready.
bonus/            — ALL bonus files. Separate Makefile/CMake target if needed.
```

## Code style

- Class names: `PascalCase`. Methods and variables: `camelCase`. Member variables: `_camelCase` (leading underscore). Macros and constants: `SCREAMING_SNAKE_CASE`.
- Namespaces: `Math::`, `RayTracer::`. Nothing lives in the global namespace except `main`.
- Headers use `#pragma once`. No include guards.
- One class per header/implementation file pair. Filename matches the class.
- Order of includes: corresponding header, then stdlib, then project headers. Blank line between groups.
- Prefer `const` by default. `constexpr` where possible. `noexcept` where genuinely true.
- Mark overrides with `override`. Mark leaf classes `final` if they really are leaves.

## Math and raytracing conventions

- Right-handed coordinate system. Camera looks down `-Z` by default.
- Colors are stored internally as `double` in `[0, 1]`, clamped and scaled to `[0, 255]` only at PPM write time.
- `Vector3D` is for directions and displacements; `Point3D` is for positions. `Point3D - Point3D = Vector3D`. `Point3D + Vector3D = Point3D`. Do not add two `Point3D`s — there is no such operation.
- Ray intersection methods write into a `HitRecord` and return `bool`. Do not return a `std::optional<HitRecord>` — the convention in this codebase is out-parameter + bool for consistency with what Epitech graders expect to see.
- `tMin` and `tMax` parameters on `hits()` — always. Defaults: `tMin = 0.001` (avoid self-intersection), `tMax = infinity`.

## Plugin contract

Every plugin `.so` exports exactly one symbol:

```cpp
extern "C" RayTracer::IPrimitive* createPrimitive();
// or
extern "C" RayTracer::ILight* createLight();
```

The plugin loader calls this, wraps the result in a `std::unique_ptr` with a custom deleter that also `dlclose`s the handle when the last plugin instance is destroyed. If you change this contract, every plugin breaks. Don't change it.

## Scene file format

We use libconfig++ syntax. A scene file has top-level groups: `camera`, `primitives`, `lights`. See `scenes/demo_sphere.cfg` for the reference. When adding a new primitive type, update both the parser in `src/scene/ConfigParser.cpp` AND create an example scene file in `scenes/`.

## Build

- `make` builds `./raytracer` and all `./plugins/*.so`.
- `make re`, `make clean`, `make fclean` required.
- CMake alternative in `CMakeLists.txt` — produces the same layout.
- Compile with `-Wall -Wextra -Werror -std=c++17`. Release: `-O2`. Debug: `-g -O0 -fsanitize=address,undefined`.

## What to do when asked to add a feature

1. Figure out which of **must / should / could** the feature is (see the project PDF).
2. Check if any **must / should** features are incomplete. If so, say so and ask whether to prioritize.
3. Identify the right module from the directory map. Do not add files outside the map's structure without justification.
4. If the feature needs a new interface or changes an existing one, flag it — this affects everyone's code.
5. Write the code, then the test, then the scene file demonstrating it.

## What to do when asked to "fix a bug"

1. Reproduce it with a minimal scene file in `scenes/`.
2. Identify whether the bug is in math, intersection logic, shading, config parsing, or the render loop. These are the five buckets. Confusing them wastes hours.
3. Fix and add a regression test.

## Things to refuse to do

- Add a library that isn't `libconfig++` or SFML.
- Link the main binary against a concrete primitive implementation.
- Use `new`/`delete` directly in new code.
- Put a `using namespace` in a header.
- Commit to `main` directly — always describe changes as a PR.
- Implement bonuses before `must` and `should` features are complete.
- Use `std::cout` for anything other than the PPM output stream. Errors → `std::cerr`. Debug logs → a proper logger macro or nothing.

## Testing

- Math classes have unit tests. Add one whenever you add a method.
- Primitives have a "does it hit a ray I know should hit it" test.
- The full pipeline is tested by rendering each scene in `scenes/` and checking the output PPM is byte-identical to a reference (or within tolerance — TBD).

## Defense readiness

At all times, the following must be true:
- `make` works from a clean clone.
- Every scene file in `scenes/` renders without crashing.
- `./screenshots/` has a pre-rendered PPM for every scene.
- The README lists every implemented feature from the PDF's must/should/could with a ✓.

If any of those are false, the next PR must fix it before adding new features.
