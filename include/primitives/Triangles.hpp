/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** Triangles
*/

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
            Triangles() = default;
            ~Triangles() = default;
            Triangles(const Triangles&) = default;
            Triangles& operator=(const Triangles&) = default;
            Triangles(const Math::Point3D& v0, const Math::Point3D& v1, const Math::Point3D& v2, std::shared_ptr<IMaterial> material) : _v0(v0), _v1(v1), _v2(v2), _material(std::move(material)) {}

            bool hits(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

        private:
            Math::Point3D _v0, _v1, _v2;
            std::shared_ptr<IMaterial> _material;

    };
}

#endif /* !TRIANGLES_HPP_ */
