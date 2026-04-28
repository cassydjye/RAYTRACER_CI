/*
** EPITECH PROJECT, 2026
** rectangle3D
** File description:
** rectangle3D
*/

#pragma once

#include "../math/Point3D.hpp"

namespace RayTracer {

    class Rectangle3D {
    public:
        Math::Point3D origin;
        Math::Vector3D bottom_side;
        Math::Vector3D left_side;

        Rectangle3D(const Math::Point3D& origin, const Math::Vector3D& bottom, const Math::Vector3D& left) : origin(origin), bottom_side(bottom), left_side(left) {}
        Math::Point3D pointAt(double u, double v) const;
    };
}
