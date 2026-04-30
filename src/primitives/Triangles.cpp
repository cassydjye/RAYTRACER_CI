/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Triangles
*/

#include "../../include/primitives/Triangles.hpp"
#include <memory>
#include <cmath>

bool RayTracer::Triangles::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    Math::Vector3D edge1 = _v1 - _v0;
    Math::Vector3D edge2 = _v2 - _v0;

    Math::Vector3D h(ray.direction.y * edge2.z - ray.direction.z * edge2.y,
                     ray.direction.z * edge2.x - ray.direction.x * edge2.z,
                     ray.direction.x * edge2.y - ray.direction.y * edge2.x);
    double a = edge1.dot(h);

    if (std::abs(a) < 1e-6)
        return false;

    double f = 1.0 / a;
    Math::Vector3D s = ray.origin - _v0;
    double u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
        return false;

    Math::Vector3D q(s.y * edge1.z - s.z * edge1.y,
                     s.z * edge1.x - s.x * edge1.z,
                     s.x * edge1.y - s.y * edge1.x);
    double v = f * ray.direction.dot(q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = f * edge2.dot(q);

    if (t < tMin || t > tMax)
        return false;

    rec.t = t;
    rec.point = ray.at(t);
    Math::Vector3D normal(edge1.y * edge2.z - edge1.z * edge2.y,
                          edge1.z * edge2.x - edge1.x * edge2.z,
                          edge1.x * edge2.y - edge1.y * edge2.x);
    rec.normal = normal.normalize();
    rec.material = _material;

    return true;
}