/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#include "Sphere.hpp"
#include <cmath>

bool RayTracer::Sphere::hits(const Ray& ray,
                              double tMin,
                              double tMax,
                              HitRecord& rec) const
{
    // Solve |P(t) - center|² = radius² → quadratic at² + bt + c = 0
    Math::Vector3D oc = ray.origin - _center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - _radius * _radius;
    double discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0)
        return false;

    double sqrtD = std::sqrt(discriminant);
    // Try nearer root first (front face), then farther (back face / inside).
    double t = (-b - sqrtD) / (2.0 * a);
    if (t < tMin || t > tMax) {
        t = (-b + sqrtD) / (2.0 * a);
        if (t < tMin || t > tMax)
            return false;
    }

    rec.t = t;
    rec.point = ray.at(t);
    rec.normal = (rec.point - _center).normalize();
    rec.material = _material;
    return true;
}
