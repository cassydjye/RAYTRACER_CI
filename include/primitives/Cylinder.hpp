/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Cylinder
*/

#include "../../include/interfaces/IPrimitive.hpp"
#include "../../include/interfaces/IMaterial.hpp"
#include "../../include/math/Point3D.hpp"
#include "../../include/math/Vector3D.hpp"

#ifndef CYLINDER_HPP_
#define CYLINDER_HPP_

namespace RayTracer {
    class Cylinder : public IPrimitive {
        public:
            Cylinder() = default;
            ~Cylinder() = default;
            Cylinder(const Cylinder&) = default;
            Cylinder(const Math::Point3D& baseCenter,
                     const Math::Vector3D& axis,
                     double radius,
                     double height,
                     std::shared_ptr<IMaterial> material)
                : _baseCenter(baseCenter), _axis(axis), _radius(radius), _height(height), _material(std::move(material)) {}
            Cylinder& operator=(const Cylinder&) = default;

            bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

        private:
            Math::Point3D _baseCenter;
            Math::Vector3D _axis;
            double _radius;
            double _height;
            std::shared_ptr<IMaterial> _material;

    };
}

#endif /* !CYLINDER_HPP_ */
