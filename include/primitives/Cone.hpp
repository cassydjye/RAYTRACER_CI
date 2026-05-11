/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Cone
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include "../interfaces/IPrimitive.hpp"
#include "../interfaces/IMaterial.hpp"
#include "../math/Point3D.hpp"
#include "../math/Vector3D.hpp"

namespace RayTracer {

    class Cone final : public IPrimitive {
    public:
        Cone() = default;
        Cone(const Math::Point3D& baseCenter,
             const Math::Vector3D& axis,
             double radius,
             double height,
             std::shared_ptr<IMaterial> material)
            : _baseCenter(baseCenter),
              _axis(axis.length() > 1e-9 ? axis.normalize() : Math::Vector3D(0.0, 1.0, 0.0)),
              _radius(radius),
              _height(height),
              _material(std::move(material)) {}
        ~Cone() override = default;
        Cone(const Cone&) = default;
        Cone& operator=(const Cone&) = default;

        bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

        std::optional<AABB> boundingBox() const override
        {
            Math::Point3D tip = _baseCenter + _axis * _height;
            double ex = _radius * std::sqrt(std::max(0.0, 1.0 - _axis.x * _axis.x));
            double ey = _radius * std::sqrt(std::max(0.0, 1.0 - _axis.y * _axis.y));
            double ez = _radius * std::sqrt(std::max(0.0, 1.0 - _axis.z * _axis.z));

            return AABB(
                Math::Point3D(std::min(_baseCenter.x - ex, tip.x),
                              std::min(_baseCenter.y - ey, tip.y),
                              std::min(_baseCenter.z - ez, tip.z)),
                Math::Point3D(std::max(_baseCenter.x + ex, tip.x),
                              std::max(_baseCenter.y + ey, tip.y),
                              std::max(_baseCenter.z + ez, tip.z))
            );
        }

    private:
        Math::Point3D _baseCenter;
        Math::Vector3D _axis = Math::Vector3D(0.0, 1.0, 0.0);
        double _radius = 1.0;
        double _height = 1.0;
        std::shared_ptr<IMaterial> _material;
    };

}
