/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IPrimitive — pure interface for scene geometry
*/

#pragma once

#include "../raytracer/Ray.hpp"
#include "../raytracer/HitRecord.hpp"

namespace RayTracer {

    // Every geometric primitive implements this interface.
    // The renderer calls hits() for each ray without knowing the concrete type.
    class IPrimitive {
    public:
        IPrimitive() = default;
        virtual ~IPrimitive() = default;
        IPrimitive(const IPrimitive&) = default;
        IPrimitive& operator=(const IPrimitive&) = default;

        // Returns true if the ray intersects this primitive with t ∈ [tMin, tMax].
        // On true, `rec` is filled with the hit distance, point, outward normal,
        // and material. On false, `rec` is unchanged.
        // Convention: tMin = 0.001 to avoid self-intersection (shadow acne).
        virtual bool hits(const Ray& ray,double tMin, double tMax, HitRecord& rec) const = 0;
    };

}
