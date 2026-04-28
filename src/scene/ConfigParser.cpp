/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConfigParser
*/

#include "../../include/scene/ConfigParser.hpp"

#include <libconfig.h++>
#include <cmath>
#include <stdexcept>
#include <string>

#include "../../include/raytracer/Camera.hpp"
#include "../../include/raytracer/Rectangle3D.hpp"
#include "../../include/math/Point3D.hpp"
#include "../../include/math/Vector3D.hpp"

#include "../../include/primitives/Sphere.hpp"
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

    // Build the virtual screen from FOV and aspect ratio.
    // Camera looks down -Z. Screen sits at z = pz - 1.
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

static void parseSpheres(const libconfig::Setting& primitives, RayTracer::Scene& scene)
{
    if (!primitives.exists("spheres"))
        return;
    const libconfig::Setting& spheres = primitives["spheres"];
    for (int i = 0; i < spheres.getLength(); ++i) {
        const libconfig::Setting& s = spheres[i];

        double x = asDouble(s["x"]);
        double y = asDouble(s["y"]);
        double z = asDouble(s["z"]);
        double r = asDouble(s["r"]);

        // Colors in the config are in [0, 255]; normalize to [0, 1].
        double cr = asDouble(s["color"]["r"]) / 255.0;
        double cg = asDouble(s["color"]["g"]) / 255.0;
        double cb = asDouble(s["color"]["b"]) / 255.0;

        auto mat = std::make_shared<RayTracer::FlatColor>(cr, cg, cb);
        scene.addPrimitive(
            std::make_unique<RayTracer::Sphere>(Math::Point3D(x, y, z), r, mat)
        );
    }
}

// ── lights ───────────────────────────────────────────────────────────────────

static void parseLights(const libconfig::Setting& lights, RayTracer::Scene& scene)
{
    if (lights.exists("ambient")) {
        double brightness = asDouble(lights["ambient"]);
        scene.addLight(std::make_unique<RayTracer::AmbientLight>(brightness));
    }

    // `diffuse` is the intensity multiplier for directional lights.
    double diffuseIntensity = 1.0;
    if (lights.exists("diffuse"))
        diffuseIntensity = asDouble(lights["diffuse"]);

    if (lights.exists("directional")) {
        const libconfig::Setting& dirs = lights["directional"];
        for (int i = 0; i < dirs.getLength(); ++i) {
            double dx = asDouble(dirs[i]["x"]);
            double dy = asDouble(dirs[i]["y"]);
            double dz = asDouble(dirs[i]["z"]);
            scene.addLight(std::make_unique<RayTracer::DirectionalLight>(
                Math::Vector3D(dx, dy, dz), diffuseIntensity
            ));
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

    try {
        auto [camera, res] = parseCamera(cfg.lookup("camera"));
        Scene scene(camera, res.first, res.second);
        if (cfg.exists("primitives"))
            parseSpheres(cfg.lookup("primitives"), scene);
        if (cfg.exists("lights"))
            parseLights(cfg.lookup("lights"), scene);
        return scene;
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
