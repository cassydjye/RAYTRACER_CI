/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere — sphere primitive implementing IPrimitive
*/

#pragma once

#include <memory>
#include "../math/Point3D.hpp"
#include "../interfaces/IPrimitive.hpp"
#include "../interfaces/IMaterial.hpp"

namespace RayTracer {

    class Sphere final : public IPrimitive {
    public:
        Sphere(const Math::Point3D& center,
               double radius,
               std::shared_ptr<IMaterial> material)
            : _center(center), _radius(radius), _material(std::move(material)) {}
        ~Sphere() override = default;
        Sphere(const Sphere&) = default;
        Sphere& operator=(const Sphere&) = default;

        bool hits(const Ray& ray,
                  double tMin,
                  double tMax,
                  HitRecord& rec) const override;

    private:
        Math::Point3D _center;
        double _radius;
        std::shared_ptr<IMaterial> _material;
    };

}
