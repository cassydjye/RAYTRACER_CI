/*
** EPITECH PROJECT, 2026
** vector
** File description:
** vector
*/

#pragma once

#include <cmath>

namespace Math {

    class Vector3D {
    public:
        double x;
        double y;
        double z;

        Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
        ~Vector3D() = default;

        double length() const;
        double dot(const Vector3D& other) const;
        Vector3D operator+(const Vector3D& other) const;
        Vector3D operator-(const Vector3D& other) const;
        Vector3D operator*(double scalar) const;
    };
}