/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightFactory — registry mapping type names to light creators
*/

#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <libconfig.h++>

#include "../interfaces/ILight.hpp"

namespace RayTracer {

    // Mirror of PrimitiveFactory for ILight.
    // Ambient light uses a scalar value in the config, so its creator receives
    // a Setting of TypeFloat/TypeInt rather than a group — the lambda handles it.
    class LightFactory {
    public:
        using Creator = std::function<std::unique_ptr<ILight>(const libconfig::Setting&)>;

        LightFactory() = default;
        ~LightFactory() = default;
        LightFactory(const LightFactory&) = default;
        LightFactory& operator=(const LightFactory&) = default;

        void registerType(const std::string& name, Creator creator)
        {
            _creators[name] = std::move(creator);
        }

        std::unique_ptr<ILight> create(const std::string& name,
                                       const libconfig::Setting& cfg) const
        {
            auto it = _creators.find(name);
            if (it == _creators.end())
                throw std::runtime_error("LightFactory: unknown type \"" + name + "\"");
            return it->second(cfg);
        }

        bool knows(const std::string& name) const
        {
            return _creators.count(name) > 0;
        }

    private:
        std::unordered_map<std::string, Creator> _creators;
    };

}
