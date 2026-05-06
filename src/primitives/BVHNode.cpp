/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** BVHNode
*/

#include "../../include/primitives/BVHNode.hpp"
#include <algorithm>

namespace RayTracer {

std::unique_ptr<BVHNode> BVHNode::build(
    std::vector<const IPrimitive*>& prims, size_t start, size_t end)
{
    auto node = std::make_unique<BVHNode>();
    size_t count = end - start;

    if (count == 1) {
        node->prim = prims[start];
        node->box  = prims[start]->boundingBox().value();
        return node;
    }

    // Find the axis with the greatest centroid spread for best splits.
    double minX = 1e18, maxX = -1e18;
    double minY = 1e18, maxY = -1e18;
    double minZ = 1e18, maxZ = -1e18;
    for (size_t i = start; i < end; ++i) {
        Math::Point3D c = prims[i]->boundingBox().value().centroid();
        if (c.x < minX) minX = c.x;
        if (c.x > maxX) maxX = c.x;
        if (c.y < minY) minY = c.y;
        if (c.y > maxY) maxY = c.y;
        if (c.z < minZ) minZ = c.z;
        if (c.z > maxZ) maxZ = c.z;
    }
    double spanX = maxX - minX, spanY = maxY - minY, spanZ = maxZ - minZ;
    int axis = (spanX > spanY && spanX > spanZ) ? 0 : (spanY > spanZ ? 1 : 2);

    std::sort(prims.begin() + start, prims.begin() + end,
        [axis](const IPrimitive* a, const IPrimitive* b) {
            Math::Point3D ca = a->boundingBox().value().centroid();
            Math::Point3D cb = b->boundingBox().value().centroid();
            double va = (axis == 0) ? ca.x : (axis == 1) ? ca.y : ca.z;
            double vb = (axis == 0) ? cb.x : (axis == 1) ? cb.y : cb.z;
            return va < vb;
        });

    size_t mid = start + count / 2;
    node->left  = build(prims, start, mid);
    node->right = build(prims, mid,   end);
    node->box   = AABB::merge(node->left->box, node->right->box);
    return node;
}

bool BVHNode::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    if (!box.hit(ray, tMin, tMax)) return false;
    if (prim) return prim->hits(ray, tMin, tMax, rec);

    // Test left first; if it hits, shrink tMax so right only finds something closer.
    bool hitL = left  && left->hit(ray, tMin, tMax, rec);
    bool hitR = right && right->hit(ray, tMin, hitL ? rec.t : tMax, rec);
    return hitL || hitR;
}

bool BVHNode::hitAny(const Ray& ray, double tMin, double tMax) const
{
    if (!box.hit(ray, tMin, tMax)) return false;
    if (prim) {
        HitRecord dummy;
        return prim->hits(ray, tMin, tMax, dummy);
    }
    if (left  && left->hitAny(ray,  tMin, tMax)) return true;
    if (right && right->hitAny(ray, tMin, tMax)) return true;
    return false;
}

} // namespace RayTracer
