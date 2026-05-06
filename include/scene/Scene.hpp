/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Scene — composite container for all scene objects (Composite pattern)
*/

#pragma once

#include <vector>
#include <memory>
#include "../raytracer/Camera.hpp"
#include "../interfaces/IPrimitive.hpp"
#include "../interfaces/ILight.hpp"

namespace RayTracer {

    // Owns all primitives and lights in the scene, plus camera and image resolution.
    // Implements the Composite pattern: the renderer queries primitives uniformly
    // through IPrimitive without knowing concrete types.
    // Non-copyable because unique_ptr members can't be copied.
    class Scene {
    public:
        Scene(const Camera& camera, int width, int height, int samples = 1)
            : _camera(camera), _width(width), _height(height), _samples(samples) {}
        ~Scene() = default;
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = default;
        Scene& operator=(Scene&&) = default;

        void addPrimitive(std::unique_ptr<IPrimitive> p) { _primitives.push_back(std::move(p)); }
        void addLight(std::unique_ptr<ILight> l) { _lights.push_back(std::move(l)); }

        const Camera& getCamera() const { return _camera; }
        int getWidth() const { return _width; }
        int getHeight() const { return _height; }
        int getSamples() const { return _samples; }
        const std::vector<std::unique_ptr<IPrimitive>>& getPrimitives() const { return _primitives; }
        const std::vector<std::unique_ptr<ILight>>& getLights() const { return _lights; }

    private:
        Camera _camera;
        int _width;
        int _height;
        int _samples;
        std::vector<std::unique_ptr<IPrimitive>> _primitives;
        std::vector<std::unique_ptr<ILight>> _lights;
    };

}
