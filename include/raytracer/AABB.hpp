/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** AABB — axis-aligned bounding box with slab intersection
*/

#pragma once

#include <algorithm>
#include "../math/Point3D.hpp"
#include "Ray.hpp"

namespace RayTracer {

struct AABB {
    Math::Point3D min, max;

    AABB(const Math::Point3D& mn, const Math::Point3D& mx) : min(mn), max(mx) {}

    bool hit(const Ray& ray, double tMin, double tMax) const noexcept {
        {
            double invD = 1.0 / ray.direction.x;
            double t0 = (min.x - ray.origin.x) * invD;
            double t1 = (max.x - ray.origin.x) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            if (t0 > tMin) tMin = t0;
            if (t1 < tMax) tMax = t1;
            if (tMax <= tMin) return false;
        }
        {
            double invD = 1.0 / ray.direction.y;
            double t0 = (min.y - ray.origin.y) * invD;
            double t1 = (max.y - ray.origin.y) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            if (t0 > tMin) tMin = t0;
            if (t1 < tMax) tMax = t1;
            if (tMax <= tMin) return false;
        }
        {
            double invD = 1.0 / ray.direction.z;
            double t0 = (min.z - ray.origin.z) * invD;
            double t1 = (max.z - ray.origin.z) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            if (t0 > tMin) tMin = t0;
            if (t1 < tMax) tMax = t1;
            if (tMax <= tMin) return false;
        }
        return true;
    }

    static AABB merge(const AABB& a, const AABB& b) noexcept {
        return AABB(
            Math::Point3D(std::min(a.min.x, b.min.x),
                          std::min(a.min.y, b.min.y),
                          std::min(a.min.z, b.min.z)),
            Math::Point3D(std::max(a.max.x, b.max.x),
                          std::max(a.max.y, b.max.y),
                          std::max(a.max.z, b.max.z))
        );
    }

    Math::Point3D centroid() const noexcept {
        return Math::Point3D(
            (min.x + max.x) * 0.5,
            (min.y + max.y) * 0.5,
            (min.z + max.z) * 0.5
        );
    }
};

} // namespace RayTracer
