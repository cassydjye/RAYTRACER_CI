/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConfigParser — loads a libconfig++ scene file and builds a Scene
*/

#pragma once

#include <string>
#include "Scene.hpp"

namespace RayTracer {

    // Implements the Builder pattern: reads a libconfig++ .cfg file and assembles
    // a fully populated Scene (camera + primitives + lights).
    //
    // Expected top-level groups: camera, primitives, lights.
    // See scenes/demo_sphere.cfg for the reference format.
    // Throws std::runtime_error on any missing required field or parse error.
    class ConfigParser {
    public:
        ConfigParser() = default;
        ~ConfigParser() = default;
        ConfigParser(const ConfigParser&) = default;
        ConfigParser& operator=(const ConfigParser&) = default;

        Scene parse(const std::string& filename) const;
    };

}
