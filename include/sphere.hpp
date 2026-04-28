/*
** EPITECH PROJECT, 2026
** sphere
** File description:
** sphere
*/

#pragma once

#include "Ray.hpp"
#include "Point3D.hpp"

namespace RayTracer {

    class Sphere {
    public:
        Math::Point3D center;
        double radius;

        Sphere(const Math::Point3D& center, double radius) : center(center), radius(radius) {}
        ~Sphere() = default;

        // Returns true if the ray intersects the sphere (discriminant > 0). No hit details.
        bool hits(const Ray& ray) const;

        // Returns true and writes the nearest positive intersection distance into `t`.
        // Uses tMin = 0.001 to avoid self-intersection ("shadow acne").
        // Use ray.at(t) to get the hit point, then (hitPoint - center).normalize() for the normal.
        bool hits(const Ray& ray, double& t) const;
    };

}