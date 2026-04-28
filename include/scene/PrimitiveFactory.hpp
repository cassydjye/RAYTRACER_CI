/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PrimitiveFactory — registry mapping type names to primitive creators
*/

#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <libconfig.h++>

#include "../interfaces/IPrimitive.hpp"

namespace RayTracer {

    // Factory pattern: callers register a creator function per type name,
    // then call create() by name without knowing concrete classes.
    // When the plugin system exists, PluginLoader will call registerType()
    // instead of the hard-coded lambdas in ConfigParser.
    class PrimitiveFactory {
    public:
        using Creator = std::function<std::unique_ptr<IPrimitive>(const libconfig::Setting&)>;

        PrimitiveFactory() = default;
        ~PrimitiveFactory() = default;
        PrimitiveFactory(const PrimitiveFactory&) = default;
        PrimitiveFactory& operator=(const PrimitiveFactory&) = default;

        void registerType(const std::string& name, Creator creator)
        {
            _creators[name] = std::move(creator);
        }

        std::unique_ptr<IPrimitive> create(const std::string& name, const libconfig::Setting& cfg) const
        {
            auto it = _creators.find(name);
            if (it == _creators.end())
                throw std::runtime_error("PrimitiveFactory: unknown type \"" + name + "\"");
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
