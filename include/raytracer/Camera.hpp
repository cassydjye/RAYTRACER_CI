/*
** EPITECH PROJECT, 2026
** camera
** File description:
** camera
*/

#pragma once

#include "../math/Point3D.hpp"
#include "Rectangle3D.hpp"
#include "Ray.hpp"

namespace RayTracer {

    class Camera {
        public:
            Math::Point3D origin;
            Rectangle3D screen;

            Camera(const Math::Point3D& origin, const Rectangle3D& screen) : origin(origin), screen(screen) {}
            ~Camera() = default;
            Ray ray(double u, double v) const;
    };
}