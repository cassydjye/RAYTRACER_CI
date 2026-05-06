/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Color
*/

#pragma once

namespace RayTracer {

    // RGB colour stored in [0, 1] double range.
    // Clamping to [0, 255] happens only at PPM write time.
    struct Color {
        double r = 0.0;
        double g = 0.0;
        double b = 0.0;

        Color() = default;
        Color(double r, double g, double b) : r(r), g(g), b(b) {}

        Color operator+(const Color& o) const noexcept { return {r + o.r, g + o.g, b + o.b}; }
        Color& operator+=(const Color& o) noexcept { r += o.r; g += o.g; b += o.b; return *this; }
        Color operator*(double s) const noexcept { return {r * s, g * s, b * s}; }
        Color operator/(double s) const noexcept { return {r / s, g / s, b / s}; }
        // Component-wise multiply (apply material colour to light colour).
        Color operator*(const Color& o) const noexcept { return {r * o.r, g * o.g, b * o.b}; }
    };

}
