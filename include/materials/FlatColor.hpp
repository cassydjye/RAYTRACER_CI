/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** FlatColor — single solid colour material, no shading variation
*/

#pragma once

#include "../interfaces/IMaterial.hpp"

namespace RayTracer {

    // The simplest material: a constant colour unaffected by surface orientation.
    // Lighting intensity is applied by the renderer on top of this colour.
    class FlatColor final : public IMaterial {
    public:
        explicit FlatColor(const Color& color) : _color(color) {}
        FlatColor(double r, double g, double b) : _color(r, g, b) {}
        FlatColor(double r, double g, double b, double specular, double shininess)
            : _color(r, g, b), _specular(specular), _shininess(shininess) {}
        ~FlatColor() override = default;
        FlatColor(const FlatColor&) = default;
        FlatColor& operator=(const FlatColor&) = default;

        Color getColor() const override { return _color; }
        double getSpecular() const override { return _specular; }
        double getShininess() const override { return _shininess; }

    private:
        Color  _color;
        double _specular  = 0.0;
        double _shininess = 32.0;
    };

}
