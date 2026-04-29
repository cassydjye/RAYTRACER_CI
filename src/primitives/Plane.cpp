/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Plane
*/

#include "../../include/primitives/Plane.hpp"
#include "../../include/materials/FlatColor.hpp"
#include <cmath>

bool RayTracer::Plane::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    double denom = _normal.dot(ray.direction);

    if (std::abs(denom) < 1e-6)
        return false;

    double t = (_point - ray.origin).dot(_normal) / denom;

    if (t < tMin || t > tMax)
        return false;

    rec.t = t;
    rec.point = ray.at(t);
    rec.normal = _normal;
    rec.material = _material;

    return true;
}
