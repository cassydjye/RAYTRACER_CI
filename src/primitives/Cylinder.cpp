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
#include <initializer_list>

bool RayTracer::Cylinder::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    double yMin = _baseCenter.y - _height / 2.0;
    double yMax = _baseCenter.y + _height / 2.0;

    bool hit = false;
    double tBest = tMax;

    // ── lateral surface ───────────────────────────────────────────────────────
    Math::Vector3D oc = ray.origin - _baseCenter;
    double a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
    if (std::abs(a) >= 1e-6) {
        double b = 2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z);
        double c = oc.x * oc.x + oc.z * oc.z - _radius * _radius;
        double disc = b * b - 4 * a * c;
        if (disc >= 0) {
            double sqrtD = std::sqrt(disc);
            for (double t : { (-b - sqrtD) / (2.0 * a), (-b + sqrtD) / (2.0 * a) }) {
                if (t < tMin || t >= tBest) continue;
                Math::Point3D p = ray.at(t);
                if (p.y < yMin || p.y > yMax) continue;
                Math::Vector3D n(p.x - _baseCenter.x, 0.0, p.z - _baseCenter.z);
                rec.t = t; rec.point = p; rec.normal = n.normalize();
                rec.material = _material;
                tBest = t; hit = true;
                break;
            }
        }
    }

    // ── end caps ──────────────────────────────────────────────────────────────
    if (std::abs(ray.direction.y) > 1e-6) {
        for (double capY : { yMin, yMax }) {
            double t = (capY - ray.origin.y) / ray.direction.y;
            if (t < tMin || t >= tBest) continue;
            Math::Point3D p = ray.at(t);
            double dx = p.x - _baseCenter.x, dz = p.z - _baseCenter.z;
            if (dx * dx + dz * dz > _radius * _radius) continue;
            double ny = (capY == yMax) ? 1.0 : -1.0;
            // Flip to face the incoming ray.
            if (ny * ray.direction.y > 0.0) ny = -ny;
            rec.t = t; rec.point = p;
            rec.normal = Math::Vector3D(0.0, ny, 0.0);
            rec.material = _material;
            tBest = t; hit = true;
        }
    }

    return hit;
}