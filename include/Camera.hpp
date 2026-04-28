/*
** EPITECH PROJECT, 2026
** camera
** File description:
** camera
*/

#pragma once

#include "Point3D.hpp"
#include "Rectangle3D.hpp"
#include "Ray.hpp"

namespace RayTracer {

    class Camera {
        public:
            Math::Point3D origin;
            Rectangle3D screen;

            Camera(const Math::Point3D& origin, const Rectangle3D& screen) : origin(origin), screen(screen) {}
            ~Camera() = default;

            // Generates the ray that passes through pixel (u, v) on the virtual screen.
            // u and v are normalised to [0, 1]: (0,0) = bottom-left, (1,1) = top-right.
            // The ray direction is NOT normalised — normalise before shading if needed.
            Ray ray(double u, double v) const;
    };
}