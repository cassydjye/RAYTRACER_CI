/*
** EPITECH PROJECT, 2026
** ray
** File description:
** ray
*/

#pragma once

#include "Vector.hpp"
#include "Point3D.hpp"

namespace RayTracer {

    class Ray {
    public:
        Math::Point3D origin;
        Math::Vector3D direction;

        Ray(const Math::Point3D& origin, const Math::Vector3D& direction) : origin(origin), direction(direction) {}
        ~Ray() = default;

        // Returns the 3D position along the ray at parameter t: P(t) = origin + t·direction.
        // t > 0 is in front of the camera; t < 0 is behind.
        Math::Point3D at(double t) const { return origin + direction * t; }
    };
}