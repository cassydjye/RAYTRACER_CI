/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** ObjLoader — loads a .obj file into a Mesh
*/

#pragma once

#include <string>
#include <memory>
#include "../primitives/Mesh.hpp"
#include "../interfaces/IMaterial.hpp"
#include "../math/Point3D.hpp"

namespace RayTracer {

    class ObjLoader {
    public:
        // Loads a .obj file and returns a Mesh.
        // All vertices are offset by `position` and share `material`.
        static Mesh load(const std::string& path,
                         const Math::Point3D& position,
                         std::shared_ptr<IMaterial> material);
    };

}
