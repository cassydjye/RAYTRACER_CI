/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder — fluent builder for assembling a Scene step by step
*/

#pragma once

#include <memory>
#include <optional>
#include <stdexcept>

#include "Scene.hpp"
#include "../raytracer/Camera.hpp"
#include "../interfaces/IPrimitive.hpp"
#include "../interfaces/ILight.hpp"

namespace RayTracer {

    // Builder pattern: accumulates camera, resolution, primitives, and lights
    // independently, then produces a fully-constructed Scene via build().
    // Throws std::runtime_error from build() if the camera was never set.
    class SceneBuilder {
    public:
        SceneBuilder() = default;
        ~SceneBuilder() = default;
        SceneBuilder(const SceneBuilder&) = delete;
        SceneBuilder& operator=(const SceneBuilder&) = delete;
        SceneBuilder(SceneBuilder&&) = default;
        SceneBuilder& operator=(SceneBuilder&&) = default;

        SceneBuilder& setCamera(const Camera& camera, int width, int height)
        {
            _camera = camera;
            _width  = width;
            _height = height;
            _hasCamera = true;
            return *this;
        }

        SceneBuilder& addPrimitive(std::unique_ptr<IPrimitive> p)
        {
            _primitives.push_back(std::move(p));
            return *this;
        }

        SceneBuilder& addLight(std::unique_ptr<ILight> l)
        {
            _lights.push_back(std::move(l));
            return *this;
        }

        Scene build()
        {
            if (!_hasCamera)
                throw std::runtime_error("SceneBuilder: camera must be set before build()");
            Scene scene(*_camera, _width, _height);
            for (auto& p : _primitives)
                scene.addPrimitive(std::move(p));
            for (auto& l : _lights)
                scene.addLight(std::move(l));
            _primitives.clear();
            _lights.clear();
            _hasCamera = false;
            return scene;
        }

    private:
        std::optional<Camera> _camera;
        int _width  = 0;
        int _height = 0;
        bool _hasCamera = false;
        std::vector<std::unique_ptr<IPrimitive>> _primitives;
        std::vector<std::unique_ptr<ILight>>     _lights;
    };

}
