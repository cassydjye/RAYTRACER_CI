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
#include <random>
#include <vector>
#include "../../include/raytracer/Ray.hpp"
#include "../../include/interfaces/IMaterial.hpp"
#include "../../include/primitives/BVHNode.hpp"

// ── helpers ──────────────────────────────────────────────────────────────────

int RayTracer::Renderer::toChannel(double v)
{
    return static_cast<int>(std::clamp(v, 0.0, 1.0) * 255.99);
}

// ── ray colour ───────────────────────────────────────────────────────────────

RayTracer::Color RayTracer::Renderer::traceRay(
    const Ray& ray, const Scene& scene,
    const BVHNode* bvh,
    const std::vector<const IPrimitive*>& unbounded,
    int depth) const
{
    HitRecord closest;
    double tMax = std::numeric_limits<double>::infinity();
    bool anyHit = false;

    // BVH traversal for bounded primitives.
    if (bvh) {
        HitRecord rec;
        if (bvh->hit(ray, 0.001, tMax, rec)) {
            anyHit  = true;
            tMax    = rec.t;
            closest = rec;
        }
    }
    // Linear scan for unbounded primitives (planes, etc.).
    for (const IPrimitive* prim : unbounded) {
        HitRecord rec;
        if (prim->hits(ray, 0.001, tMax, rec)) {
            anyHit  = true;
            tMax    = rec.t;
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
        if (bvh && bvh->hitAny(shadowRay, 0.001, std::numeric_limits<double>::infinity()))
            inShadow = true;
        if (!inShadow) {
            for (const IPrimitive* prim : unbounded) {
                HitRecord shadowHit;
                if (prim->hits(shadowRay, 0.001, std::numeric_limits<double>::infinity(), shadowHit)) {
                    inShadow = true;
                    break;
                }
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
    const double fogMin = 0.40;
    double fadedIntensity = fogMin + (totalIntensity - fogMin) * fogFactor;

    Color localColor = baseColor * fadedIntensity + specularSum;

    // Mirror reflection: blend local shading with reflected ray color.
    double kr = closest.material->getReflectivity();
    if (kr > 0.0 && depth < 8) {
        Math::Vector3D reflDir = ray.direction.normalize()
            - closest.normal * (2.0 * closest.normal.dot(ray.direction.normalize()));
        Ray reflRay(closest.point, reflDir);
        Color reflColor = traceRay(reflRay, scene, bvh, unbounded, depth + 1);
        localColor = localColor * (1.0 - kr) + reflColor * kr;
    }

    return localColor;
}

// ── render loop ──────────────────────────────────────────────────────────────

void RayTracer::Renderer::render(const Scene& scene, std::ostream& out) const
{
    int w       = scene.getWidth();
    int h       = scene.getHeight();
    int samples = scene.getSamples();
    const Camera& cam = scene.getCamera();

    // Split primitives: bounded ones go into the BVH, infinite ones stay linear.
    std::vector<const IPrimitive*> bounded, unbounded;
    for (const auto& p : scene.getPrimitives()) {
        if (p->boundingBox().has_value())
            bounded.push_back(p.get());
        else
            unbounded.push_back(p.get());
    }
    std::unique_ptr<BVHNode> bvhRoot;
    if (!bounded.empty())
        bvhRoot = BVHNode::build(bounded, 0, bounded.size());
    const BVHNode* bvh = bvhRoot.get();

    // Render into a flat buffer so threads write independent rows without races.
    std::vector<Color> buffer(static_cast<size_t>(w * h));

    #pragma omp parallel for schedule(dynamic)
    for (int y = h - 1; y >= 0; --y) {
        // Each thread gets its own RNG seeded by row to avoid contention.
        std::mt19937 rng(static_cast<unsigned>(y * 1000003));
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int x = 0; x < w; ++x) {
            Color accum(0.0, 0.0, 0.0);
            for (int s = 0; s < samples; ++s) {
                double u = (x + (samples > 1 ? dist(rng) : 0.5)) / (w - 1);
                double v = (y + (samples > 1 ? dist(rng) : 0.5)) / (h - 1);
                accum += traceRay(cam.ray(u, v), scene, bvh, unbounded);
            }
            buffer[static_cast<size_t>((h - 1 - y) * w + x)] = accum / static_cast<double>(samples);
        }
    }

    out << "P3\n" << w << " " << h << "\n255\n";
    for (const Color& c : buffer)
        out << toChannel(c.r) << " " << toChannel(c.g) << " " << toChannel(c.b) << "\n";
}
