/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AmbientLight — uniform background illumination with no direction
*/

#pragma once

#include "../interfaces/ILight.hpp"

namespace RayTracer {

    // Adds a constant brightness to every surface regardless of orientation.
    // Prevents completely black shadow regions.
    class AmbientLight final : public ILight {
    public:
        // `brightness` in [0, 1]: how much ambient light this source contributes.
        explicit AmbientLight(double brightness) : _brightness(brightness) {}
        ~AmbientLight() override = default;
        AmbientLight(const AmbientLight&) = default;
        AmbientLight& operator=(const AmbientLight&) = default;

        double getAmbient() const override { return _brightness; }
        bool isAmbient() const override { return true; }

        void illuminate(const Math::Point3D&,
                        Math::Vector3D&,
                        double& intensity) const override
        {
            intensity = _brightness;
        }

    private:
        double _brightness;
    };

}
