/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IMaterial — pure interface for surface materials
*/

#pragma once

#include "../raytracer/Color.hpp"

namespace RayTracer {

    // Every material must implement this interface.
    // The renderer calls getColor() to obtain the base colour, then applies
    // lighting on top. Future materials (textured, reflective, etc.) extend here.
    class IMaterial {
    public:
        IMaterial() = default;
        virtual ~IMaterial() = default;
        IMaterial(const IMaterial&) = default;
        IMaterial& operator=(const IMaterial&) = default;

        // Returns the intrinsic colour of the surface at the current hit point.
        virtual Color getColor() const = 0;

        // Phong specular coefficient [0, 1]. Default: no specular.
        virtual double getSpecular() const { return 0.0; }

        // Phong shininess exponent. Higher = tighter highlight.
        virtual double getShininess() const { return 32.0; }
    };

}
