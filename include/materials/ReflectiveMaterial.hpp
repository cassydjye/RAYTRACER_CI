/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** ReflectiveMaterial — surface with mirror reflectivity
*/

#pragma once

#include "../interfaces/IMaterial.hpp"

namespace RayTracer {

    class ReflectiveMaterial final : public IMaterial {
    public:
        ReflectiveMaterial(double r, double g, double b,
                           double reflectivity,
                           double specular  = 0.0,
                           double shininess = 32.0)
            : _color(r, g, b), _reflectivity(reflectivity),
              _specular(specular), _shininess(shininess) {}
        ~ReflectiveMaterial() override = default;
        ReflectiveMaterial(const ReflectiveMaterial&) = default;
        ReflectiveMaterial& operator=(const ReflectiveMaterial&) = default;

        Color  getColor()        const override { return _color; }
        double getReflectivity() const override { return _reflectivity; }
        double getSpecular()     const override { return _specular; }
        double getShininess()    const override { return _shininess; }

    private:
        Color  _color;
        double _reflectivity;
        double _specular;
        double _shininess;
    };

}
