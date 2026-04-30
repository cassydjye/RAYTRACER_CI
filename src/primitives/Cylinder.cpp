/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Cylinder
*/

#include "../../include/primitives/Cylinder.hpp"
#include "../../include/materials/FlatColor.hpp"
#include "../../include/interfaces/IMaterial.hpp"
#include "../../include/raytracer/HitRecord.hpp"
#include "../../include/raytracer/Ray.hpp"
#include "../../include/math/Point3D.hpp"
#include "../../include/math/Vector3D.hpp"
#include <memory>
#include <cmath>

bool RayTracer::Cylinder::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    Math::Vector3D oc = ray.origin - _baseCenter;

    double a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
    if (std::abs(a) < 1e-6)
        return false;

    double b = 2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z);
    double c = oc.x * oc.x + oc.z * oc.z - _radius * _radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;

    double sqrtD = std::sqrt(discriminant);

    double t = (-b - sqrtD) / (2.0 * a);
    if (t < tMin || t > tMax) {
        t = (-b + sqrtD) / (2.0 * a);
        if (t < tMin || t > tMax)
            return false;
    }

    Math::Point3D p = ray.at(t);

    double yMin = _baseCenter.y - _height / 2.0;
    double yMax = _baseCenter.y + _height / 2.0;

    if (p.y < yMin || p.y > yMax)
        return false;

    rec.t = t;
    rec.point = p;

    Math::Vector3D normal = p - _baseCenter;
    normal.y = 0;
    rec.normal = normal.normalize();

    rec.material = _material;

    return true;
}