/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** HitRecord
*/

#pragma once

#include <memory>
#include "Point3D.hpp"
#include "Vector.hpp"

namespace RayTracer {

    class IMaterial;

    // Filled by IPrimitive::hits() when an intersection is found.
    // Always check the bool return before reading any field.
    struct HitRecord {
        double t = 0.0;
        Math::Point3D point{0.0, 0.0, 0.0};
        // Outward unit normal at the hit point (points away from the surface).
        Math::Vector3D normal{0.0, 0.0, 0.0};
        std::shared_ptr<IMaterial> material;
    };

}
