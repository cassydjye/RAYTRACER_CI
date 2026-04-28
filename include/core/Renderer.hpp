/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Renderer — walks every pixel, casts a ray, shades the result
*/

#pragma once

#include <ostream>
#include "../scene/Scene.hpp"
#include "../raytracer/Color.hpp"

namespace RayTracer {

    class Renderer {
    public:
        Renderer() = default;
        ~Renderer() = default;
        Renderer(const Renderer&) = default;
        Renderer& operator=(const Renderer&) = default;

        // Renders `scene` in full and writes a PPM P3 image to `out`.
        // Iterates every pixel, calls traceRay(), applies Lambertian shading.
        void render(const Scene& scene, std::ostream& out) const;

    private:
        // Returns the colour seen along `ray` given the scene's geometry and lights.
        Color traceRay(const Ray& ray, const Scene& scene) const;

        // Clamps `v` to [0, 1] and maps to [0, 255].
        static int toChannel(double v);
    };

}
