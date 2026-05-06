/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** BVHNode — bounding volume hierarchy node for accelerated ray intersection
*/

#pragma once

#include <memory>
#include <vector>
#include "../../include/raytracer/AABB.hpp"
#include "../../include/interfaces/IPrimitive.hpp"
#include "../../include/raytracer/HitRecord.hpp"

namespace RayTracer {

struct BVHNode {
    AABB                     box{Math::Point3D(0,0,0), Math::Point3D(0,0,0)};
    std::unique_ptr<BVHNode> left, right;
    const IPrimitive*        prim = nullptr;  // non-null only for leaf nodes

    // Build a BVH over prims[start..end). Reorders that slice in-place.
    static std::unique_ptr<BVHNode> build(
        std::vector<const IPrimitive*>& prims, size_t start, size_t end);

    // Returns the closest hit in [tMin, tMax], updating rec.
    bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const;

    // Returns true on any hit — skips recording details (used for shadow rays).
    bool hitAny(const Ray& ray, double tMin, double tMax) const;
};

} // namespace RayTracer
