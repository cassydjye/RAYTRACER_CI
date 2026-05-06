/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Mesh
*/

#include "../../include/primitives/Mesh.hpp"

bool RayTracer::Mesh::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
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
