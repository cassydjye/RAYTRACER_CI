/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Plane
*/

#pragma once

#include <memory>
#include "../math/Vector3D.hpp"
#include "../interfaces/IPrimitive.hpp"
#include "../interfaces/IMaterial.hpp"

#ifndef PLANE_HPP_
#define PLANE_HPP_

namespace RayTracer {

    class Plane : public IPrimitive {
        public:
            Plane(const Math::Point3D& point, const Math::Vector3D& normal, std::shared_ptr<IMaterial> material) : _point(point), _normal(normal), _material(std::move(material)) {}

            Plane() = default;
            ~Plane() = default;
            Plane(const Plane&) = default;
            Plane& operator=(const Plane&) = default;

            bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

        private:
            Math::Point3D _point;
            Math::Vector3D _normal;
            std::shared_ptr<IMaterial> _material;

    };
}

#endif /* !PLANE_HPP_ */
