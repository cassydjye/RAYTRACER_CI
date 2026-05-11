/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Cone
*/

#include "../../include/primitives/Cone.hpp"
#include "../../include/raytracer/HitRecord.hpp"
#include "../../include/raytracer/Ray.hpp"
#include <cmath>

bool RayTracer::Cone::hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
    constexpr double eps = 1e-6;

    if (_radius <= 0.0 || _height <= 0.0)
        return false;

    Math::Point3D apex = _baseCenter + _axis * _height;
    Math::Vector3D co = ray.origin - apex;
    double slope = _radius / _height;
    double slope2 = slope * slope;
    double du = ray.direction.dot(_axis);
    double cou = co.dot(_axis);
    double a = ray.direction.dot(ray.direction) - (1.0 + slope2) * du * du;
    double b = 2.0 * (co.dot(ray.direction) - (1.0 + slope2) * cou * du);
    double c = co.dot(co) - (1.0 + slope2) * cou * cou;
    bool hit = false;
    double tBest = tMax;

    auto trySideHit = [&](double t) {
        if (t < tMin || t >= tBest)
            return;
        Math::Point3D point = ray.at(t);
        Math::Vector3D q = point - apex;
        double m = q.dot(_axis);

        if (m < -_height || m > 0.0)
            return;
        Math::Vector3D normal = q - _axis * ((1.0 + slope2) * m);
        if (normal.length() < eps)
            return;
        rec.t = t;
        rec.point = point;
        rec.normal = normal.normalize();
        rec.material = _material;
        tBest = t;
        hit = true;
    };

    if (std::abs(a) < eps) {
        if (std::abs(b) >= eps)
            trySideHit(-c / b);
    } else {
        double discriminant = b * b - 4.0 * a * c;
        if (discriminant >= 0.0) {
            double sqrtD = std::sqrt(discriminant);
            double t0 = (-b - sqrtD) / (2.0 * a);
            double t1 = (-b + sqrtD) / (2.0 * a);

            if (t0 > t1)
                std::swap(t0, t1);
            trySideHit(t0);
            trySideHit(t1);
        }
    }

    double denom = ray.direction.dot(_axis);
    if (std::abs(denom) > eps) {
        double t = (_baseCenter - ray.origin).dot(_axis) / denom;

        if (t >= tMin && t < tBest) {
            Math::Point3D point = ray.at(t);
            Math::Vector3D fromBase = point - _baseCenter;
            Math::Vector3D radial = fromBase - _axis * fromBase.dot(_axis);

            if (radial.dot(radial) <= _radius * _radius) {
                rec.t = t;
                rec.point = point;
                rec.normal = _axis * -1.0;
                rec.material = _material;
                hit = true;
            }
        }
    }

    return hit;
}
