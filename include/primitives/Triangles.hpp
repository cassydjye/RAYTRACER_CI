/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Triangles
*/

#include <algorithm>
#include <initializer_list>
#include "../../include/interfaces/IPrimitive.hpp"
#include "../../include/materials/FlatColor.hpp"
#include "../../include/interfaces/IMaterial.hpp"
#include "../../include/raytracer/HitRecord.hpp"
#include "../../include/raytracer/Ray.hpp"
#include "../../include/math/Point3D.hpp"
#include "../../include/math/Vector3D.hpp"

#ifndef TRIANGLES_HPP_
#define TRIANGLES_HPP_

namespace RayTracer {
    class Triangles : public IPrimitive {
        public:
            ~Triangles() = default;
            Triangles(const Triangles&) = default;
            Triangles& operator=(const Triangles&) = default;

            // Flat shading — normal computed from geometry.
            Triangles(const Math::Point3D& v0, const Math::Point3D& v1, const Math::Point3D& v2,
                      std::shared_ptr<IMaterial> material)
                : _v0(v0), _v1(v1), _v2(v2), _material(std::move(material)),
                  _hasVertexNormals(false),
                  _n0(0,0,0), _n1(0,0,0), _n2(0,0,0) {}

            // Phong shading — per-vertex normals interpolated at hit point.
            Triangles(const Math::Point3D& v0, const Math::Point3D& v1, const Math::Point3D& v2,
                      const Math::Vector3D& n0, const Math::Vector3D& n1, const Math::Vector3D& n2,
                      std::shared_ptr<IMaterial> material)
                : _v0(v0), _v1(v1), _v2(v2), _material(std::move(material)),
                  _hasVertexNormals(true),
                  _n0(n0), _n1(n1), _n2(n2) {}

            bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

            std::optional<AABB> boundingBox() const override {
                const double e = 1e-4;
                return AABB(
                    Math::Point3D(std::min({_v0.x, _v1.x, _v2.x}) - e,
                                  std::min({_v0.y, _v1.y, _v2.y}) - e,
                                  std::min({_v0.z, _v1.z, _v2.z}) - e),
                    Math::Point3D(std::max({_v0.x, _v1.x, _v2.x}) + e,
                                  std::max({_v0.y, _v1.y, _v2.y}) + e,
                                  std::max({_v0.z, _v1.z, _v2.z}) + e)
                );
            }

        private:
            Math::Point3D  _v0, _v1, _v2;
            std::shared_ptr<IMaterial> _material;
            bool           _hasVertexNormals;
            Math::Vector3D _n0, _n1, _n2;
    };
}

#endif /* !TRIANGLES_HPP_ */
