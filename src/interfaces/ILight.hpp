/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ILight — pure interface for light sources
*/

#pragma once

#include "Point3D.hpp"
#include "Vector.hpp"

namespace RayTracer {

    // Every light source implements this interface.
    // Two flavours exist: ambient (no direction, adds flat brightness) and
    // directional / point (has a direction, drives diffuse shading).
    class ILight {
    public:
        ILight() = default;
        virtual ~ILight() = default;
        ILight(const ILight&) = default;
        ILight& operator=(const ILight&) = default;

        // Returns the ambient contribution [0, 1] of this light.
        // Directional lights return 0.
        virtual double getAmbient() const = 0;

        // True for lights that contribute uniformly regardless of surface orientation.
        virtual bool isAmbient() const = 0;

        // Fills `direction` (unit vector FROM the surface TOWARD the light)
        // and `intensity` [0, 1]. Undefined for ambient lights.
        virtual void illuminate(const Math::Point3D& point,
                                Math::Vector3D& direction,
                                double& intensity) const = 0;
    };

}
