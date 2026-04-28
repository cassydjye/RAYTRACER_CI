/*
** EPITECH PROJECT, 2026
** ray
** File description:
** ray
*/

#pragma once

#include "../math/Vector3D.hpp"
#include "../math/Point3D.hpp"

namespace RayTracer {

    class Ray {
    public:
        Math::Point3D origin;
        Math::Vector3D direction;

        Ray(const Math::Point3D& origin, const Math::Vector3D& direction) : origin(origin), direction(direction) {}
        ~Ray() = default;
    };
}