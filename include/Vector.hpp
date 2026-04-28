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

        // Euclidean magnitude: sqrt(x²+y²+z²)
        double length() const;

        // Scalar projection: returns |this|·|other|·cos(θ). Used for lighting (normal · lightDir).
        double dot(const Vector3D& other) const;

        Vector3D operator+(const Vector3D& other) const;
        Vector3D operator-(const Vector3D& other) const;
        Vector3D operator*(double scalar) const;
        Vector3D operator/(double scalar) const;

        // Returns a unit vector in the same direction. Undefined if length() == 0.
        Vector3D normalize() const;
    };
}