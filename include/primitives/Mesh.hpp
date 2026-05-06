/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Mesh — a collection of triangles treated as one primitive
*/

#pragma once

#include <vector>
#include <memory>
#include "../../include/interfaces/IPrimitive.hpp"
#include "../../include/primitives/Triangles.hpp"

namespace RayTracer {

    class Mesh : public IPrimitive {
    public:
        explicit Mesh(std::vector<Triangles> triangles)
            : _triangles(std::move(triangles)) {}

        bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

    private:
        std::vector<Triangles> _triangles;
    };

}
