/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** PointLight — positional light with inverse-square falloff
*/

#pragma once

#include "../interfaces/ILight.hpp"
#include <cmath>

namespace RayTracer {

    class PointLight final : public ILight {
    public:
        // `position`: world-space position of the light bulb.
        // `intensity`: base brightness before distance falloff.
        PointLight(const Math::Point3D& position, double intensity)
            : _position(position), _intensity(intensity) {}
        ~PointLight() override = default;
        PointLight(const PointLight&) = default;
        PointLight& operator=(const PointLight&) = default;

        double getAmbient() const override { return 0.0; }
        bool isAmbient() const override { return false; }

        void illuminate(const Math::Point3D& point,
                        Math::Vector3D& direction,
                        double& intensity) const override
        {
            Math::Vector3D toLight(
                _position.x - point.x,
                _position.y - point.y,
                _position.z - point.z
            );
            double dist = toLight.length();
            direction = toLight / dist;
            intensity = _intensity / (dist * dist);
        }

    private:
        Math::Point3D _position;
        double        _intensity;
    };

}
