/*
** EPITECH PROJECT, 2026
** RAYTRACER_CI
** File description:
** test_sphere
*/

#include <criterion/criterion.h>
#include <iostream>
#include <sstream>
#include <string>
#include "../include/Help.hpp"
#include "../include/primitives/Cylinder.hpp"
#include "../include/primitives/Sphere.hpp"
#include "../include/raytracer/Ray.hpp"
#include "../include/math/Point3D.hpp"
#include "../include/math/Vector3D.hpp"
#include "../include/raytracer/HitRecord.hpp"
#include "../include/materials/FlatColor.hpp"
#include "../include/interfaces/IMaterial.hpp"
#include "../include/interfaces/IPrimitive.hpp"
#include "../include/scene/Scene.hpp"
#include "../include/scene/ConfigParser.hpp"

using namespace RayTracer;

/* ----------------------------- */
/* Dummy material for testing    */
/* ----------------------------- */

class DummyMaterial : public IMaterial {
public:
    Color getColor() const override
    {
        return Color(255, 255, 255);
    }
};

/* ----------------------------- */
/* Helper                        */
/* ----------------------------- */

static Sphere makeSphere()
{
    return Sphere(
        Math::Point3D(0, 0, 0),
        1.0,
        std::make_shared<DummyMaterial>()
    );
}

/* ========================================================= */
/*                        HIT TESTS                          */
/* ========================================================= */

Test(Sphere, RayHitsSphere)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, -5),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 1000.0, rec);

    cr_assert(hit);

    cr_assert_float_eq(rec.t, 4.0, 1e-6);

    cr_assert_float_eq(rec.point.x, 0.0, 1e-6);
    cr_assert_float_eq(rec.point.y, 0.0, 1e-6);
    cr_assert_float_eq(rec.point.z, -1.0, 1e-6);

    cr_assert_float_eq(rec.normal.x, 0.0, 1e-6);
    cr_assert_float_eq(rec.normal.y, 0.0, 1e-6);
    cr_assert_float_eq(rec.normal.z, -1.0, 1e-6);
}

/* ========================================================= */
/*                        MISS TEST                          */
/* ========================================================= */

Test(Sphere, RayMissesSphere)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, -5),
        Math::Vector3D(0, 1, 0)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 1000.0, rec);

    cr_assert_not(hit);
}

/* ========================================================= */
/*                      TANGENT TEST                         */
/* ========================================================= */

Test(Sphere, TangentRay)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(1, 0, -5),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 1000.0, rec);

    cr_assert(hit);

    cr_assert_float_eq(rec.t, 5.0, 1e-6);

    cr_assert_float_eq(rec.point.x, 1.0, 1e-6);
    cr_assert_float_eq(rec.point.y, 0.0, 1e-6);
    cr_assert_float_eq(rec.point.z, 0.0, 1e-6);
}

/* ========================================================= */
/*                  RAY STARTS INSIDE TEST                   */
/* ========================================================= */

Test(Sphere, RayStartsInsideSphere)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, 0),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 1000.0, rec);

    cr_assert(hit);

    cr_assert_float_eq(rec.t, 1.0, 1e-6);

    cr_assert_float_eq(rec.point.x, 0.0, 1e-6);
    cr_assert_float_eq(rec.point.y, 0.0, 1e-6);
    cr_assert_float_eq(rec.point.z, 1.0, 1e-6);

    cr_assert_float_eq(rec.normal.x, 0.0, 1e-6);
    cr_assert_float_eq(rec.normal.y, 0.0, 1e-6);
    cr_assert_float_eq(rec.normal.z, 1.0, 1e-6);
}

/* ========================================================= */
/*                     TMIN REJECTION                        */
/* ========================================================= */

Test(Sphere, RejectHitBelowTMin)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, -5),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 6.1, 1000.0, rec);

    cr_assert_not(hit);
}

/* ========================================================= */
/*                     TMAX REJECTION                        */
/* ========================================================= */

Test(Sphere, RejectHitAboveTMax)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, -5),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 3.0, rec);

    cr_assert_not(hit);
}

/* ========================================================= */
/*                  SPHERE BEHIND RAY                        */
/* ========================================================= */

Test(Sphere, SphereBehindRay)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, 5),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 1000.0, rec);

    cr_assert_not(hit);
}

/* ========================================================= */
/*                    BOUNDING BOX TEST                      */
/* ========================================================= */

Test(Sphere, BoundingBox)
{
    Sphere sphere(
        Math::Point3D(1, 2, 3),
        2.0,
        std::make_shared<DummyMaterial>()
    );

    auto box = sphere.boundingBox();

    cr_assert(box.has_value());

    cr_assert_float_eq(box->min.x, -1.0, 1e-6);
    cr_assert_float_eq(box->min.y,  0.0, 1e-6);
    cr_assert_float_eq(box->min.z,  1.0, 1e-6);

    cr_assert_float_eq(box->max.x, 3.0, 1e-6);
    cr_assert_float_eq(box->max.y, 4.0, 1e-6);
    cr_assert_float_eq(box->max.z, 5.0, 1e-6);
}

/* ========================================================= */
/*                    LARGE SPHERE TEST                      */
/* ========================================================= */

Test(Sphere, LargeSphere)
{
    Sphere sphere(
        Math::Point3D(0, 0, 0),
        1000.0,
        std::make_shared<DummyMaterial>()
    );

    Ray ray(
        Math::Point3D(0, 0, -2000),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 5000.0, rec);

    cr_assert(hit);

    cr_assert_float_eq(rec.t, 1000.0, 1e-3);
}

/* ========================================================= */
/*                     VERY CLOSE RAY                        */
/* ========================================================= */

Test(Sphere, VeryCloseRay)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, -1.001),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.0001, 1000.0, rec);

    cr_assert(hit);

    cr_assert(rec.t > 0.0);
}

/* ========================================================= */
/*                  NORMAL IS NORMALIZED                     */
/* ========================================================= */

Test(Sphere, NormalIsNormalized)
{
    Sphere sphere = makeSphere();

    Ray ray(
        Math::Point3D(0, 0, -5),
        Math::Vector3D(0, 0, 1)
    );

    HitRecord rec;

    bool hit = sphere.hits(ray, 0.001, 1000.0, rec);

    cr_assert(hit);

    double length = std::sqrt(
        rec.normal.x * rec.normal.x +
        rec.normal.y * rec.normal.y +
        rec.normal.z * rec.normal.z
    );

    cr_assert_float_eq(length, 1.0, 1e-6);
}