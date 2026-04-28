/*
** EPITECH PROJECT, 2026
** point3D
** File description:
** point3D
*/

#pragma once

#include "Vector.hpp"

namespace Math {

    class Point3D {
    public:
        double x;
        double y;
        double z;

        Point3D() = default;
        Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
        ~Point3D() = default;
        Point3D operator+(const Vector3D& vec) const;
        Vector3D operator-(const Point3D& other) const;
    };
}