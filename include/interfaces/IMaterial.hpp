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
    };

}
