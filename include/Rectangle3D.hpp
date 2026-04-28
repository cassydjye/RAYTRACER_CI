/*
** EPITECH PROJECT, 2026
** rectangle3D
** File description:
** rectangle3D
*/

#pragma once

#include "Point3D.hpp"

namespace RayTracer {

    class Rectangle3D {
    public:
        Math::Point3D origin;
        Math::Vector3D bottom_side;
        Math::Vector3D left_side;

        // `origin` is the bottom-left corner of the virtual screen.
        // `bottom` spans the full width (u-axis), `left` spans the full height (v-axis).
        Rectangle3D(const Math::Point3D& origin, const Math::Vector3D& bottom, const Math::Vector3D& left) : origin(origin), bottom_side(bottom), left_side(left) {}

        // Maps normalised coordinates (u, v) ∈ [0,1]² to a world-space point on the screen.
        // Used by Camera::ray() to aim a ray through a specific pixel.
        Math::Point3D pointAt(double u, double v) const;
    };
}
