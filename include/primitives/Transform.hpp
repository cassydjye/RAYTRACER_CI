/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Transform — rotation wrapper for any IPrimitive
*/

#pragma once

#include <memory>
#include <array>
#include "../../include/interfaces/IPrimitive.hpp"
#include "../../include/math/Point3D.hpp"
#include "../../include/math/Vector3D.hpp"

namespace RayTracer {

    // Wraps any IPrimitive with an XYZ Euler rotation around a pivot point.
    // Rotation angles are in radians.
    class Transform : public IPrimitive {
    public:
        // rx/ry/rz: rotation in radians around X, Y, Z axes (applied in that order).
        // pivot: the world-space point to rotate around.
        Transform(std::unique_ptr<IPrimitive> inner,
                  const Math::Point3D& pivot,
                  double rx, double ry, double rz);

        bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

    private:
        std::unique_ptr<IPrimitive> _inner;
        Math::Point3D _pivot;

        // Row-major 3x3 rotation matrix R and its transpose R^T (= inverse for orthogonal R).
        std::array<double, 9> _R;   // world-from-object: transforms object vectors to world
        std::array<double, 9> _Rt;  // object-from-world: transforms world vectors to object

        static Math::Vector3D applyMatrix(const std::array<double, 9>& m, const Math::Vector3D& v);
    };

}
