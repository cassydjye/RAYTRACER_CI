/*
** EPITECH PROJECT, 2026
** point3D
** File description:
** point3D
*/

#include "../include/Point3D.hpp"

Math::Point3D Math::Point3D::operator+(const Math::Vector3D& vec) const
{
    return Point3D(x + vec.x, y + vec.y, z + vec.z);
}

Math::Vector3D Math::Point3D::operator-(const Math::Point3D& other) const
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}
