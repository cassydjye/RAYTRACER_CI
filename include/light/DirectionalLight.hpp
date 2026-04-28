/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DirectionalLight — light rays all parallel, coming from a fixed direction
*/

#pragma once

#include "../interfaces/ILight.hpp"

namespace RayTracer {

    // Models a light source so far away that all its rays are parallel (like the sun).
    // `direction` in the constructor is the direction FROM which the light comes
    // (i.e. the vector pointing toward the light source).
    class DirectionalLight final : public ILight {
    public:
        // `toLight`: unit vector pointing from the scene toward the light source.
        // `intensity`: diffuse contribution in [0, 1].
        DirectionalLight(const Math::Vector3D& toLight, double intensity)
            : _toLight(toLight.normalize()), _intensity(intensity) {}
        ~DirectionalLight() override = default;
        DirectionalLight(const DirectionalLight&) = default;
        DirectionalLight& operator=(const DirectionalLight&) = default;

        double getAmbient() const override { return 0.0; }
        bool isAmbient() const override { return false; }

        // `direction` is filled with the unit vector FROM the surface TOWARD the light.
        void illuminate(const Math::Point3D&,
                        Math::Vector3D& direction,
                        double& intensity) const override
        {
            direction = _toLight;
            intensity = _intensity;
        }

    private:
        Math::Vector3D _toLight;
        double _intensity;
    };

}
