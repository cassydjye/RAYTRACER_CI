/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Renderer — walks every pixel, casts a ray, shades the result
*/

#pragma once

#include <ostream>
#include <vector>
#include "../scene/Scene.hpp"
#include "../raytracer/Color.hpp"

namespace RayTracer {

    class BVHNode;
    class IPrimitive;

    class Renderer {
    public:
        Renderer() = default;
        ~Renderer() = default;
        Renderer(const Renderer&) = default;
        Renderer& operator=(const Renderer&) = default;

        void render(const Scene& scene, std::ostream& out) const;

    private:
        Color traceRay(const Ray& ray, const Scene& scene,
                       const BVHNode* bvh,
                       const std::vector<const IPrimitive*>& unbounded) const;

        static int toChannel(double v);
    };

}
