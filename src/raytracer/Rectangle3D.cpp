/*
** EPITECH PROJECT, 2026
** rectangle3D
** File description:
** rectangle3D
*/

#include "../../include/raytracer/Rectangle3D.hpp"

Math::Point3D RayTracer::Rectangle3D::pointAt(double u, double v) const
{
    return origin + bottom_side * u + left_side * v;
}