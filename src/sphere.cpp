/*
** EPITECH PROJECT, 2026
** sphere
** File description:
** sphere
*/

#include "../include/sphere.hpp"
#include <cmath>

bool RayTracer::Sphere::hits(const Ray& ray) const
{
    // Solve |P(t) - center|² = radius² where P(t) = origin + t·direction.
    // Expanding gives the quadratic at²+bt+c=0; discriminant > 0 means two real intersections.
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
    double b = 2.0 * (oc.x * ray.direction.x + oc.y * ray.direction.y + oc.z * ray.direction.z);
    double c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - radius * radius;
    double discriminant = b * b - 4 * a * c;

    return (discriminant > 0);
}

bool RayTracer::Sphere::hits(const Ray& ray, double& t) const
{
    // Same quadratic as above, but we also solve for t so the caller can compute
    // the hit point and surface normal for shading.
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;
    double sqrtD = std::sqrt(discriminant);
    // Try the nearer root first (front face), fall back to the farther one (back face / inside).
    double t0 = (-b - sqrtD) / (2.0 * a);
    double t1 = (-b + sqrtD) / (2.0 * a);
    if (t0 > 0.001) { t = t0; return true; }
    if (t1 > 0.001) { t = t1; return true; }
    return false;
}