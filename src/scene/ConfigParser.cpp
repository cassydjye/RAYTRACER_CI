/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConfigParser
*/

#include "../../include/scene/ConfigParser.hpp"
#include "../../include/scene/SceneBuilder.hpp"
#include "../../include/scene/PrimitiveFactory.hpp"
#include "../../include/scene/LightFactory.hpp"

#include <libconfig.h++>
#include <cmath>
#include <stdexcept>
#include <string>

#include "../../include/raytracer/Camera.hpp"
#include "../../include/raytracer/Rectangle3D.hpp"
#include "../../include/math/Point3D.hpp"
#include "../../include/math/Vector3D.hpp"

#include "../../include/primitives/Sphere.hpp"
#include "../../include/primitives/Plane.hpp"
#include "../../include/primitives/Cylinder.hpp"
#include "../../include/primitives/Triangles.hpp"
#include "../../include/light/AmbientLight.hpp"
#include "../../include/light/DirectionalLight.hpp"
#include "../../include/materials/FlatColor.hpp"

// ── helpers ──────────────────────────────────────────────────────────────────

static double asDouble(const libconfig::Setting& s)
{
    if (s.getType() == libconfig::Setting::TypeFloat)
        return static_cast<double>(s);
    if (s.getType() == libconfig::Setting::TypeInt)
        return static_cast<double>(static_cast<int>(s));
    throw std::runtime_error(std::string("expected a number at: ") + s.getPath());
}

// ── factory registration ──────────────────────────────────────────────────────

static RayTracer::PrimitiveFactory makePrimitiveFactory()
{
    RayTracer::PrimitiveFactory f;

    f.registerType("sphere", [](const libconfig::Setting& s) {
        double x = asDouble(s["x"]);
        double y = asDouble(s["y"]);
        double z = asDouble(s["z"]);
        double r = asDouble(s["r"]);
        // Colors in the config are in [0, 255]; normalize to [0, 1].
        double cr = asDouble(s["color"]["r"]) / 255.0;
        double cg = asDouble(s["color"]["g"]) / 255.0;
        double cb = asDouble(s["color"]["b"]) / 255.0;
        auto mat = std::make_shared<RayTracer::FlatColor>(cr, cg, cb);
        return std::make_unique<RayTracer::Sphere>(Math::Point3D(x, y, z), r, mat);
    });

    f.registerType("plane", [](const libconfig::Setting& s) {
        double x = asDouble(s["x"]);
        double y = asDouble(s["y"]);
        double z = asDouble(s["z"]);
        double nx = asDouble(s["nx"]);
        double ny = asDouble(s["ny"]);
        double nz = asDouble(s["nz"]);
        // Colors in the config are in [0, 255]; normalize to [0, 1].
        double cr = asDouble(s["color"]["r"]) / 255.0;
        double cg = asDouble(s["color"]["g"]) / 255.0;
        double cb = asDouble(s["color"]["b"]) / 255.0;
        auto mat = std::make_shared<RayTracer::FlatColor>(cr, cg, cb);
        return std::make_unique<RayTracer::Plane>(
            Math::Point3D(x, y, z), Math::Vector3D(nx, ny, nz), mat
        );
    });

    f.registerType("cylinder", [](const libconfig::Setting& s) {
        double x = asDouble(s["x"]);
        double y = asDouble(s["y"]);
        double z = asDouble(s["z"]);
        double ax = asDouble(s["ax"]);
        double ay = asDouble(s["ay"]);
        double az = asDouble(s["az"]);
        double r = asDouble(s["r"]);
        double h = asDouble(s["h"]);
        // Colors in the config are in [0, 255]; normalize to [0, 1].
        double cr = asDouble(s["color"]["r"]) / 255.0;
        double cg = asDouble(s["color"]["g"]) / 255.0;
        double cb = asDouble(s["color"]["b"]) / 255.0;
        auto mat = std::make_shared<RayTracer::FlatColor>(cr, cg, cb);
        return std::make_unique<RayTracer::Cylinder>(
            Math::Point3D(x, y, z), Math::Vector3D(ax, ay, az), r, h, mat
        );
    });

    f.registerType("triangle", [](const libconfig::Setting& s) {
        double v0x = asDouble(s["v0"]["x"]);
        double v0y = asDouble(s["v0"]["y"]);
        double v0z = asDouble(s["v0"]["z"]);
        double v1x = asDouble(s["v1"]["x"]);
        double v1y = asDouble(s["v1"]["y"]);
        double v1z = asDouble(s["v1"]["z"]);
        double v2x = asDouble(s["v2"]["x"]);
        double v2y = asDouble(s["v2"]["y"]);
        double v2z = asDouble(s["v2"]["z"]);
        // Colors in the config are in [0, 255]; normalize to [0, 1].
        double cr = asDouble(s["color"]["r"]) / 255.0;
        double cg = asDouble(s["color"]["g"]) / 255.0;
        double cb = asDouble(s["color"]["b"]) / 255.0;
        auto mat = std::make_shared<RayTracer::FlatColor>(cr, cg, cb);
        return std::make_unique<RayTracer::Triangles>(
            Math::Point3D(v0x, v0y, v0z),
            Math::Point3D(v1x, v1y, v1z),
            Math::Point3D(v2x, v2y, v2z),
            mat
        );
    });

    return f;
}

static RayTracer::LightFactory makeLightFactory()
{
    RayTracer::LightFactory f;

    f.registerType("ambient", [](const libconfig::Setting& s) {
        return std::make_unique<RayTracer::AmbientLight>(asDouble(s));
    });

    f.registerType("directional", [](const libconfig::Setting& s) {
        double dx = asDouble(s["x"]);
        double dy = asDouble(s["y"]);
        double dz = asDouble(s["z"]);
        double intensity = s.exists("intensity") ? asDouble(s["intensity"]) : 1.0;
        return std::make_unique<RayTracer::DirectionalLight>(
            Math::Vector3D(dx, dy, dz), intensity
        );
    });

    return f;
}

// ── camera ───────────────────────────────────────────────────────────────────

static std::pair<RayTracer::Camera, std::pair<int,int>>
parseCamera(const libconfig::Setting& cam)
{
    int width  = cam["resolution"]["width"];
    int height = cam["resolution"]["height"];

    double px = asDouble(cam["position"]["x"]);
    double py = asDouble(cam["position"]["y"]);
    double pz = asDouble(cam["position"]["z"]);

    double fov = asDouble(cam["fieldOfView"]);

    double aspect   = static_cast<double>(width) / static_cast<double>(height);
    double halfH    = std::tan((fov * M_PI / 180.0) / 2.0);
    double halfW    = aspect * halfH;

    Math::Point3D  origin(px, py, pz);
    Math::Point3D  screenOrigin(px - halfW, py - halfH, pz - 1.0);
    Math::Vector3D bottomSide(2.0 * halfW, 0.0, 0.0);
    Math::Vector3D leftSide(0.0, 2.0 * halfH, 0.0);

    RayTracer::Rectangle3D screen(screenOrigin, bottomSide, leftSide);
    return {RayTracer::Camera(origin, screen), {width, height}};
}

// ── primitives ───────────────────────────────────────────────────────────────

static void parsePrimitives(const libconfig::Setting& primitives,
                             RayTracer::SceneBuilder& builder,
                             const RayTracer::PrimitiveFactory& factory)
{
    // Each child group name is a type key (e.g. "spheres" → type "sphere").
    // Convention: config key = type name + "s" (plural).
    for (int g = 0; g < primitives.getLength(); ++g) {
        const libconfig::Setting& group = primitives[g];
        std::string typeName = group.getName();
        // Strip trailing 's' to get the singular type name.
        if (!typeName.empty() && typeName.back() == 's')
            typeName.pop_back();
        if (!factory.knows(typeName))
            throw std::runtime_error("parsePrimitives: no factory for type \"" + typeName + "\"");
        for (int i = 0; i < group.getLength(); ++i)
            builder.addPrimitive(factory.create(typeName, group[i]));
    }
}

// ── lights ───────────────────────────────────────────────────────────────────

static void parseLights(const libconfig::Setting& lights,
                        RayTracer::SceneBuilder& builder,
                        const RayTracer::LightFactory& factory,
                        double diffuseIntensity)
{
    if (lights.exists("ambient"))
        builder.addLight(factory.create("ambient", lights["ambient"]));

    if (lights.exists("directional")) {
        const libconfig::Setting& dirs = lights["directional"];
        for (int i = 0; i < dirs.getLength(); ++i) {
            // Inject the scene-level diffuse intensity if the entry has none.
            if (!dirs[i].exists("intensity")) {
                double dx = asDouble(dirs[i]["x"]);
                double dy = asDouble(dirs[i]["y"]);
                double dz = asDouble(dirs[i]["z"]);
                builder.addLight(std::make_unique<RayTracer::DirectionalLight>(
                    Math::Vector3D(dx, dy, dz), diffuseIntensity
                ));
            } else {
                builder.addLight(factory.create("directional", dirs[i]));
            }
        }
    }
}

// ── public entry point ───────────────────────────────────────────────────────

RayTracer::Scene RayTracer::ConfigParser::parse(const std::string& filename) const
{
    libconfig::Config cfg;
    try {
        cfg.readFile(filename.c_str());
    } catch (const libconfig::FileIOException&) {
        throw std::runtime_error("Cannot open scene file: " + filename);
    } catch (const libconfig::ParseException& e) {
        throw std::runtime_error(
            std::string("Parse error in ") + e.getFile() +
            " at line " + std::to_string(e.getLine()) +
            ": " + e.getError()
        );
    }

    PrimitiveFactory primFactory  = makePrimitiveFactory();
    LightFactory     lightFactory = makeLightFactory();
    SceneBuilder     builder;

    try {
        auto [camera, res] = parseCamera(cfg.lookup("camera"));
        builder.setCamera(camera, res.first, res.second);

        if (cfg.exists("primitives"))
            parsePrimitives(cfg.lookup("primitives"), builder, primFactory);

        if (cfg.exists("lights")) {
            double diffuseIntensity = 1.0;
            if (cfg.lookup("lights").exists("diffuse"))
                diffuseIntensity = asDouble(cfg.lookup("lights")["diffuse"]);
            parseLights(cfg.lookup("lights"), builder, lightFactory, diffuseIntensity);
        }

        return builder.build();
    } catch (const libconfig::SettingNotFoundException& e) {
        throw std::runtime_error(
            std::string("Missing required setting: ") + e.getPath()
        );
    } catch (const libconfig::SettingTypeException& e) {
        throw std::runtime_error(
            std::string("Wrong type for setting: ") + e.getPath()
        );
    }
}
