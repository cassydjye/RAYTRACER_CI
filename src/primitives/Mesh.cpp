/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Mesh
*/

#include "../../include/primitives/Mesh.hpp"

RayTracer::Mesh::Mesh(std::vector<Triangles> triangles)
    : _triangles(std::move(triangles))
{
    if (_triangles.empty()) return;
    std::vector<const IPrimitive*> ptrs;
    ptrs.reserve(_triangles.size());
    for (const auto& t : _triangles)
        ptrs.push_back(&t);
    _bvh = BVHNode::build(ptrs, 0, ptrs.size());
}

bool RayTracer::Mesh::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    if (_bvh) return _bvh->hit(ray, tMin, tMax, rec);

    bool hit = false;
    double tBest = tMax;
    HitRecord tmp;
    for (const auto& tri : _triangles) {
        if (tri.hits(ray, tMin, tBest, tmp)) {
            tBest = tmp.t;
            rec   = tmp;
            hit   = true;
        }
    }
    return hit;
}

std::optional<RayTracer::AABB> RayTracer::Mesh::boundingBox() const
{
    if (_bvh) return _bvh->box;
    return std::nullopt;
}
