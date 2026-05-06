/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IPrimitive — pure interface for scene geometry
*/

#pragma once

#include <optional>
#include "../raytracer/Ray.hpp"
#include "../raytracer/HitRecord.hpp"
#include "../raytracer/AABB.hpp"

namespace RayTracer {

    class IPrimitive {
    public:
        IPrimitive() = default;
        virtual ~IPrimitive() = default;
        IPrimitive(const IPrimitive&) = default;
        IPrimitive& operator=(const IPrimitive&) = default;

        virtual bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const = 0;

        // Returns the world-space AABB. nullopt for infinite primitives (e.g. planes).
        virtual std::optional<AABB> boundingBox() const { return std::nullopt; }
    };

}
