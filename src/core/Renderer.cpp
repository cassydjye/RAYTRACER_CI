/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Renderer
*/

#include "../../include/core/Renderer.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include "../../include/raytracer/Ray.hpp"
#include "../../include/interfaces/IMaterial.hpp"

// ── helpers ──────────────────────────────────────────────────────────────────

int RayTracer::Renderer::toChannel(double v)
{
    return static_cast<int>(std::clamp(v, 0.0, 1.0) * 255.99);
}

// ── ray colour ───────────────────────────────────────────────────────────────

RayTracer::Color RayTracer::Renderer::traceRay(const Ray& ray, const Scene& scene) const
{
    HitRecord closest;
    double tMax = std::numeric_limits<double>::infinity();
    bool anyHit = false;

    // Find the closest primitive hit along this ray.
    for (const auto& prim : scene.getPrimitives()) {
        HitRecord rec;
        if (prim->hits(ray, 0.001, tMax, rec)) {
            anyHit = true;
            tMax   = rec.t;
            closest = rec;
        }
    }

    if (!anyHit) {
        // Sky gradient: white at horizon, light blue at top.
        double blend = 0.5 * (ray.direction.normalize().y + 1.0);
        return Color(
            0.53 + 0.47 * blend,
            0.81 + 0.19 * blend,
            0.92 + 0.08 * blend
        );
    }

    Color  baseColor = closest.material->getColor();
    double ks        = closest.material->getSpecular();
    double shininess = closest.material->getShininess();

    // View direction: from hit point toward camera (opposite of ray direction).
    Math::Vector3D viewDir = ray.direction.normalize() * -1.0;

    // Accumulate ambient from all ambient lights.
    double ambientSum = 0.0;
    for (const auto& light : scene.getLights()) {
        if (light->isAmbient())
            ambientSum += light->getAmbient();
    }

    // Accumulate diffuse + specular from all directional lights.
    double diffuseSum  = 0.0;
    Color  specularSum(0.0, 0.0, 0.0);
    for (const auto& light : scene.getLights()) {
        if (light->isAmbient())
            continue;
        Math::Vector3D toLight{0.0, 0.0, 0.0};
        double intensity = 0.0;
        light->illuminate(closest.point, toLight, intensity);

        Ray shadowRay(closest.point, toLight);
        bool inShadow = false;
        for (const auto& prim : scene.getPrimitives()) {
            HitRecord shadowHit;
            if (prim->hits(shadowRay, 0.001, std::numeric_limits<double>::infinity(), shadowHit)) {
                inShadow = true;
                break;
            }
        }
        if (inShadow)
            continue;

        double diffuse = std::max(0.0, closest.normal.dot(toLight));
        diffuseSum += diffuse * intensity;

        // Phong specular: reflect toLight around normal, dot with viewDir.
        if (ks > 0.0) {
            Math::Vector3D refl = closest.normal * (2.0 * closest.normal.dot(toLight)) - toLight;
            double spec = std::pow(std::max(0.0, refl.dot(viewDir)), shininess);
            specularSum += Color(1.0, 1.0, 1.0) * (ks * spec * intensity);
        }
    }

    double totalIntensity = std::min(1.0, ambientSum + diffuseSum);

    double distance = closest.t * ray.direction.length();
    double fogFactor = std::exp(-0.02 * distance);
    const double fogMin = 0.40; // Minimum intensity at infinite distance (controls fog density).
    double fadedIntensity = fogMin + (totalIntensity - fogMin) * fogFactor;

    return baseColor * fadedIntensity + specularSum;
}

// ── render loop ──────────────────────────────────────────────────────────────

void RayTracer::Renderer::render(const Scene& scene, std::ostream& out) const
{
    int w = scene.getWidth();
    int h = scene.getHeight();
    const Camera& cam = scene.getCamera();

    out << "P3\n" << w << " " << h << "\n255\n";

    // Rows from top (y = h-1) to bottom (y = 0) so PPM row 0 = image top.
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            double u = static_cast<double>(x) / (w - 1);
            double v = static_cast<double>(y) / (h - 1);
            Ray r = cam.ray(u, v);
            Color c = traceRay(r, scene);
            out << toChannel(c.r) << " "
                << toChannel(c.g) << " "
                << toChannel(c.b) << "\n";
        }
    }
}
