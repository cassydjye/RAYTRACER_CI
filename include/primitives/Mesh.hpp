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
#include "../../include/primitives/BVHNode.hpp"

namespace RayTracer {

    class Mesh : public IPrimitive {
    public:
        explicit Mesh(std::vector<Triangles> triangles);
        ~Mesh() = default;
        Mesh(Mesh&&) = default;
        Mesh& operator=(Mesh&&) = default;

        bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;
        std::optional<AABB> boundingBox() const override;

    private:
        std::vector<Triangles>   _triangles;
        std::unique_ptr<BVHNode> _bvh;
    };

}
