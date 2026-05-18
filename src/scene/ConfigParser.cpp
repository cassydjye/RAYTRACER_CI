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
#include "../../include/primitives/Cone.hpp"
#include "../../include/primitives/Triangles.hpp"
#include "../../include/primitives/Mesh.hpp"
#include "../../include/primitives/Transform.hpp"
#include "../../include/scene/ObjLoader.hpp"
#include "../../include/light/AmbientLight.hpp"
#include "../../include/light/DirectionalLight.hpp"
#include "../../include/light/PointLight.hpp"
#include "../../include/materials/FlatColor.hpp"
<<<<<<< HEAD
#include "../../include/materials/ReflectiveMaterial.hpp"
=======
>>>>>>> 5f759d8b2f6b7c8f4269fb4c54707537d8a85965

// ── helpers ──────────────────────────────────────────────────────────────────

static double asDouble(const libconfig::Setting& s)
{
    if (s.getType() == libconfig::Setting::TypeFloat)
        return static_cast<double>(s);
    if (s.getType() == libconfig::Setting::TypeInt)
        return static_cast<double>(static_cast<int>(s));
    throw std::runtime_error(std::string("expected a number at: ") + s.getPath());
}

// ── transform wrap helper ────────────────────────────────────────────────────

static std::unique_ptr<RayTracer::IPrimitive>
wrapTransform(std::unique_ptr<RayTracer::IPrimitive> prim,
             const libconfig::Setting& s,
             const Math::Point3D& pivot)
{
    bool hasRotation = s.exists("rotation");
    bool hasTranslation = s.exists("translation");

    if (!hasRotation && !hasTranslation)
        return prim;

    const double deg = M_PI / 180.0;
    double rx = hasRotation && s["rotation"].exists("x") ? asDouble(s["rotation"]["x"]) * deg : 0.0;
    double ry = hasRotation && s["rotation"].exists("y") ? asDouble(s["rotation"]["y"]) * deg : 0.0;
    double rz = hasRotation && s["rotation"].exists("z") ? asDouble(s["rotation"]["z"]) * deg : 0.0;
    double tx = hasTranslation && s["translation"].exists("x") ? asDouble(s["translation"]["x"]) : 0.0;
    double ty = hasTranslation && s["translation"].exists("y") ? asDouble(s["translation"]["y"]) : 0.0;
    double tz = hasTranslation && s["translation"].exists("z") ? asDouble(s["translation"]["z"]) : 0.0;

    return std::make_unique<RayTracer::Transform>(
        std::move(prim), pivot, rx, ry, rz, Math::Vector3D(tx, ty, tz)
    );
}

// ── material helper ───────────────────────────────────────────────────────────

<<<<<<< HEAD
static std::shared_ptr<RayTracer::IMaterial> makeMaterial(const libconfig::Setting& s)
=======
static std::shared_ptr<RayTracer::FlatColor> makeMaterial(const libconfig::Setting& s)
>>>>>>> 5f759d8b2f6b7c8f4269fb4c54707537d8a85965
{
    double cr = asDouble(s["color"]["r"]) / 255.0;
    double cg = asDouble(s["color"]["g"]) / 255.0;
    double cb = asDouble(s["color"]["b"]) / 255.0;
<<<<<<< HEAD
    double ks           = s.exists("specular")     ? asDouble(s["specular"])     : 0.0;
    double shininess    = s.exists("shininess")    ? asDouble(s["shininess"])    : 32.0;
    double reflectivity = s.exists("reflectivity") ? asDouble(s["reflectivity"]) : 0.0;
    if (reflectivity > 0.0)
        return std::make_shared<RayTracer::ReflectiveMaterial>(cr, cg, cb, reflectivity, ks, shininess);
=======
    double ks       = s.exists("specular")  ? asDouble(s["specular"])  : 0.0;
    double shininess = s.exists("shininess") ? asDouble(s["shininess"]) : 32.0;
>>>>>>> 5f759d8b2f6b7c8f4269fb4c54707537d8a85965
    return std::make_shared<RayTracer::FlatColor>(cr, cg, cb, ks, shininess);
}

// ── factory registration ──────────────────────────────────────────────────────

static RayTracer::PrimitiveFactory makePrimitiveFactory()
{
    RayTracer::PrimitiveFactory f;

    f.registerType("sphere", [](const libconfig::Setting& s) {
        double x = asDouble(s["x"]), y = asDouble(s["y"]), z = asDouble(s["z"]);
        double r = asDouble(s["r"]);
        auto mat = makeMaterial(s);
        Math::Point3D center(x, y, z);
        auto prim = std::make_unique<RayTracer::Sphere>(center, r, mat);
        return wrapTransform(std::move(prim), s, center);
    });

    f.registerType("plane", [](const libconfig::Setting& s) {
        double x = asDouble(s["x"]), y = asDouble(s["y"]), z = asDouble(s["z"]);
        double nx = asDouble(s["nx"]), ny = asDouble(s["ny"]), nz = asDouble(s["nz"]);
        auto mat = makeMaterial(s);
        Math::Point3D point(x, y, z);
        auto prim = std::make_unique<RayTracer::Plane>(point, Math::Vector3D(nx, ny, nz), mat);
        return wrapTransform(std::move(prim), s, point);
    });

    f.registerType("cylinder", [](const libconfig::Setting& s) {
        double x = asDouble(s["x"]), y = asDouble(s["y"]), z = asDouble(s["z"]);
        double ax = asDouble(s["ax"]), ay = asDouble(s["ay"]), az = asDouble(s["az"]);
        double r = asDouble(s["r"]), h = asDouble(s["h"]);
        auto mat = makeMaterial(s);
        Math::Point3D base(x, y, z);
        auto prim = std::make_unique<RayTracer::Cylinder>(base, Math::Vector3D(ax, ay, az), r, h, mat);
        return wrapTransform(std::move(prim), s, base);
    });

    f.registerType("cone", [](const libconfig::Setting& s) {
        double x = asDouble(s["x"]), y = asDouble(s["y"]), z = asDouble(s["z"]);
        double ax = asDouble(s["ax"]), ay = asDouble(s["ay"]), az = asDouble(s["az"]);
        double r = asDouble(s["r"]), h = asDouble(s["h"]);
        auto mat = makeMaterial(s);
        Math::Point3D base(x, y, z);
        auto prim = std::make_unique<RayTracer::Cone>(base, Math::Vector3D(ax, ay, az), r, h, mat);
        return wrapTransform(std::move(prim), s, base);
    });

    f.registerType("triangle", [](const libconfig::Setting& s) {
        Math::Point3D v0(asDouble(s["v0"]["x"]), asDouble(s["v0"]["y"]), asDouble(s["v0"]["z"]));
        Math::Point3D v1(asDouble(s["v1"]["x"]), asDouble(s["v1"]["y"]), asDouble(s["v1"]["z"]));
        Math::Point3D v2(asDouble(s["v2"]["x"]), asDouble(s["v2"]["y"]), asDouble(s["v2"]["z"]));
        auto mat = makeMaterial(s);
        auto prim = std::make_unique<RayTracer::Triangles>(v0, v1, v2, mat);
        Math::Point3D centroid(
            (v0.x + v1.x + v2.x) / 3.0,
            (v0.y + v1.y + v2.y) / 3.0,
            (v0.z + v1.z + v2.z) / 3.0
        );
        return wrapTransform(std::move(prim), s, centroid);
    });

    f.registerType("mesh", [](const libconfig::Setting& s) {
        double ox = s.exists("position") ? asDouble(s["position"]["x"]) : 0.0;
        double oy = s.exists("position") ? asDouble(s["position"]["y"]) : 0.0;
        double oz = s.exists("position") ? asDouble(s["position"]["z"]) : 0.0;
        Math::Point3D position(ox, oy, oz);
        auto mat = makeMaterial(s);
        std::unique_ptr<RayTracer::Mesh> prim;

        if (s.exists("file")) {
            // Load from .obj file.
            std::string path = s["file"].c_str();
            prim = std::make_unique<RayTracer::Mesh>(
                RayTracer::ObjLoader::load(path, position, mat)
            );
        } else {
            // Inline triangle list.
            std::vector<RayTracer::Triangles> tris;
            const libconfig::Setting& triList = s["triangles"];
            for (int i = 0; i < triList.getLength(); ++i) {
                const libconfig::Setting& t = triList[i];
                Math::Point3D v0(asDouble(t["v0"]["x"]) + ox,
                                 asDouble(t["v0"]["y"]) + oy,
                                 asDouble(t["v0"]["z"]) + oz);
                Math::Point3D v1(asDouble(t["v1"]["x"]) + ox,
                                 asDouble(t["v1"]["y"]) + oy,
                                 asDouble(t["v1"]["z"]) + oz);
                Math::Point3D v2(asDouble(t["v2"]["x"]) + ox,
                                 asDouble(t["v2"]["y"]) + oy,
                                 asDouble(t["v2"]["z"]) + oz);
                auto triMat = t.exists("color") ? makeMaterial(t) : mat;
                tris.emplace_back(v0, v1, v2, triMat);
            }
            prim = std::make_unique<RayTracer::Mesh>(std::move(tris));
        }

        return wrapTransform(std::move(prim), s, position);
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

    f.registerType("point", [](const libconfig::Setting& s) {
        double px = asDouble(s["x"]);
        double py = asDouble(s["y"]);
        double pz = asDouble(s["z"]);
        double intensity = s.exists("intensity") ? asDouble(s["intensity"]) : 1.0;
        return std::make_unique<RayTracer::PointLight>(
            Math::Point3D(px, py, pz), intensity
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

    double rx = 0.0, ry = 0.0, rz = 0.0;
    if (cam.exists("rotation")) {
        const double deg = M_PI / 180.0;
        if (cam["rotation"].exists("x")) rx = asDouble(cam["rotation"]["x"]) * deg;
        if (cam["rotation"].exists("y")) ry = asDouble(cam["rotation"]["y"]) * deg;
        if (cam["rotation"].exists("z")) rz = asDouble(cam["rotation"]["z"]) * deg;
    }

    // Build R = Rz * Ry * Rx (same convention as Transform).
    double cx = std::cos(rx), sx = std::sin(rx);
    double cy = std::cos(ry), sy = std::sin(ry);
    double cz = std::cos(rz), sz = std::sin(rz);
    auto rot = [&](double vx, double vy, double vz) -> Math::Vector3D {
        double ox =  cy*cz*vx + (cz*sx*sy - cx*sz)*vy + (cx*cz*sy + sx*sz)*vz;
        double oy =  cy*sz*vx + (cx*cz + sx*sy*sz)*vy + (cx*sy*sz - cz*sx)*vz;
        double oz = -sy*vx    +  cy*sx*vy              +  cx*cy*vz;
        return Math::Vector3D(ox, oy, oz);
    };

    // Rotate the canonical screen basis vectors.
    Math::Vector3D forward = rot(0.0, 0.0, -1.0);
    Math::Vector3D right   = rot(halfW, 0.0, 0.0);
    Math::Vector3D up      = rot(0.0, halfH, 0.0);

    Math::Point3D  origin(px, py, pz);
    Math::Point3D  screenOrigin(
        px + forward.x - right.x - up.x,
        py + forward.y - right.y - up.y,
        pz + forward.z - right.z - up.z
    );
    Math::Vector3D bottomSide(2.0 * right.x, 2.0 * right.y, 2.0 * right.z);
    Math::Vector3D leftSide(2.0 * up.x, 2.0 * up.y, 2.0 * up.z);

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
        // Derive singular: try stripping "es" first (meshes→mesh), then "s".
        if (typeName.size() >= 2 && typeName.substr(typeName.size() - 2) == "es") {
            std::string candidate = typeName.substr(0, typeName.size() - 2);
            if (factory.knows(candidate)) { typeName = candidate; }
            else if (!typeName.empty() && typeName.back() == 's') typeName.pop_back();
        } else if (!typeName.empty() && typeName.back() == 's') {
            typeName.pop_back();
        }
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

    if (lights.exists("point")) {
        const libconfig::Setting& pts = lights["point"];
        for (int i = 0; i < pts.getLength(); ++i)
            builder.addLight(factory.create("point", pts[i]));
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
        int samples = 1;
        if (cfg.lookup("camera").exists("samples"))
            samples = static_cast<int>(cfg.lookup("camera")["samples"]);
        builder.setCamera(camera, res.first, res.second, samples);

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
